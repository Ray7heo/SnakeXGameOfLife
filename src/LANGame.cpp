#include "../include/LANGame.h"

#define DrawText RayDrawText
#define RayDrawText DrawText
#define RayCloseWindow CloseWindow

LANGame::LANGame(): textInput(), server(context, ""), client(context, "", ""),
                    guestButton({0, textInput.bounds.height, 200, 50}, "Guest"),
                    hostButton({0, guestButton.bounds.y + 50, 200, 50}, "Host")
{
}

LANGame::~LANGame()
{
	context.stop();
	isContextRun = false;
}

LANGame::LANGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake),
                                                              textInput({0, 0, 200, 30}, ""), server(context, "0000"),
                                                              client(context, "127.0.0.1", "12345"),
                                                              guestButton(
	                                                              {0, textInput.bounds.height, 200, 50}, "Guest"),
                                                              hostButton({0, guestButton.bounds.y + 50, 200, 50},
                                                                         "Host")
{
}

void LANGame::update()
{
	if (gameState == GameState::Playing || gameState == GameState::Paused || gameState == GameState::GameOver)
	{
		if (!snake->isDead)
		{
			snake->move();
		}
		if (!remoteSnake->isDead)
		{
			// Check collision with food
			if (remoteSnake->body.empty()) return;
			auto x = remoteSnake->body.front().x;
			auto y = remoteSnake->body.front().y;
			if (x >= 0 && x < config.gridWidth && y >= 0 && y < config.gridHeight)
			{
				const auto cell = cells[static_cast<int>(y)][static_cast<int>(x)];
				if (cell->type == CellType::Edible || cell->type == CellType::Rot)
				{
					cell->type = CellType::Blank;
				}
			}
		}
	}
	GameBase::update();
}

void LANGame::restart()
{
	GameBase::restart();
	snake = std::make_unique<
		PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
	// send
	rapidjson::Document document;
	document = rapidjson::Document(rapidjson::kObjectType);
	rapidjson::Value value = snake->toJson(document.GetAllocator());
	document.AddMember("snake", value, document.GetAllocator());
	document.AddMember("score", score, document.GetAllocator());
	document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	auto data = new std::string(buffer.GetString());
	server.send(*data, [data](const std::error_code ec, size_t t)
	{
		delete data;
	});

	while (!isMapReady)
	{
	}
	// send map info
	for (int y = 0; y < config.gridHeight; y++)
	{
		for (int x = 0; x < config.gridWidth; x++)
		{
			document = rapidjson::Document(rapidjson::kObjectType);
			document.AddMember("dataType", rapidjson::StringRef("cells"),
			                   document.GetAllocator());
			document.AddMember("x", x, document.GetAllocator());
			document.AddMember("y", y, document.GetAllocator());
			document.AddMember("cell", cells[y][x]->toJson(document.GetAllocator()),
			                   document.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);
			auto data = new std::string(buffer.GetString());
			server.send(*data, [data](const std::error_code ec, size_t t)
			{
				delete data;
			});
		}
	}
}

int LANGame::countLiveNeighbors(const int x, const int y) const
{
	int count = 0;
	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (i >= 0 && i < config.gridHeight && j >= 0 && j < config.gridWidth && !(i == x && j == y))
			{
				if (remoteSnake->body.empty()) return count + GameBase::countLiveNeighbors(x, y);
				const auto head = remoteSnake->body.front();
				const auto tail = remoteSnake->body.back();
				if ((head.x == i && head.y == j) || (tail.x == i && tail.y == j))
				{
					count++;
				}
			}
		}
	}
	return count + GameBase::countLiveNeighbors(x, y);
}

void LANGame::syncState()
{
	if (!canSyncState) return;
	canSyncState = false;
	std::thread([&]()
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			rapidjson::Document document(rapidjson::kObjectType);
			rapidjson::Value value = snake->toJson(document.GetAllocator());
			document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
			document.AddMember("state", rapidjson::StringRef("playing"), document.GetAllocator());
			document.AddMember("snake", value, document.GetAllocator());
			document.AddMember("score", score, document.GetAllocator());
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			auto data = new std::string(buffer.GetString());
			if (isHost)
			{
				server.send(*data, [data](const std::error_code ec, size_t t)
				{
					delete data;
				});
			}
			else
			{
				client.send(*data, [data](const std::error_code ec, size_t t)
				{
					delete data;
				});
			}
		}
	}).detach();
}

void LANGame::draw()
{
	if (gameState == GameState::Start)
	{
		if (canDrawRemoteSnake)
		{
			remoteSnake->draw();
		}
		if (canDrawLocalSnake)
		{
			snake->draw();
		}
		if (!canDrawLocalSnake && !canDrawRemoteSnake)
		{
			hostButton.draw();
			guestButton.draw();
			textInput.draw();
			textInput.update();
		}
		if (!isHost)
		{
			startButton.bounds.width = 0;
			startButton.text = "";
		}
		else
		{
			startButton.bounds.width = 200;
			startButton.text = "Start";
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && startButton.isClicked(GetMousePosition()))
		{
			std::thread([this]()
			{
				if (isHost)
				{
					auto document = rapidjson::Document(rapidjson::kObjectType);
					document.AddMember("dataType", rapidjson::StringRef("state"), document.GetAllocator());
					document.AddMember("state", rapidjson::StringRef("playing"), document.GetAllocator());
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					document.Accept(writer);
					auto data = new std::string(buffer.GetString());
					server.send(*data, [data](const std::error_code ec, size_t t)
					{
						delete data;
					});
				}
				else
				{
					gameState = GameState::Start;
				}
			}).detach();
			syncState();
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hostButton.isClicked(GetMousePosition()))
		{
			if (!isContextRun)
			{
				isHost = true;
				isContextRun = true;
				canDrawLocalSnake = true;
				std::thread([this]
				{
					server = UdpServer(context, "12345");
					server.receive([&](const std::string& data)
					{
						rapidjson::Document document;
						document = rapidjson::Document();
						document.Parse(data.c_str());
						if (document["dataType"] == "snake")
						{
							if (document["state"] == "start")
							{
								// parse remote snake
								remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
								remoteSnake->config = config;
								canDrawRemoteSnake = true;
								// send
								document = rapidjson::Document(rapidjson::kObjectType);
								rapidjson::Value value = snake->toJson(document.GetAllocator());
								document.AddMember("snake", value, document.GetAllocator());
								document.AddMember("score", score, document.GetAllocator());
								document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
								rapidjson::StringBuffer buffer;
								rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
								document.Accept(writer);
								auto data = new std::string(buffer.GetString());
								server.send(*data, [data](const std::error_code ec, size_t t)
								{
									delete data;
								});

								while (!isMapReady)
								{
								}
								// send map info
								for (int y = 0; y < config.gridHeight; y++)
								{
									for (int x = 0; x < config.gridWidth; x++)
									{
										document = rapidjson::Document(rapidjson::kObjectType);
										document.AddMember("dataType", rapidjson::StringRef("cells"),
										                   document.GetAllocator());
										document.AddMember("x", x, document.GetAllocator());
										document.AddMember("y", y, document.GetAllocator());
										document.AddMember("cell", cells[y][x]->toJson(document.GetAllocator()),
										                   document.GetAllocator());
										rapidjson::StringBuffer buffer;
										rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
										document.Accept(writer);
										auto data = new std::string(buffer.GetString());
										server.send(*data, [data](const std::error_code ec, size_t t)
										{
											delete data;
										});
									}
								}
							}
							else if (document["state"] == "playing")
							{
								remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
								remoteSnake->config = config;
								remoteScore = document["score"].GetInt();
							}
						}
						else if (document["dataType"] == "state")
						{
							if (document["state"] == "start")
							{
								gameState = GameState::Start;
							}
							else if (document["state"] == "menu")
							{
								gameState = GameState::Menu;
							}
							else
							{
								gameState = GameState::Playing;
							}
						}
					});
					context.run();
				}).detach();
			}
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && guestButton.isClicked(GetMousePosition()))
		{
			if (!isContextRun)
			{
				isHost = false;
				canDrawLocalSnake = true;
				isContextRun = true;
				std::thread([this]
				{
					client = UdpClient(context, textInput.text, "12345");

					rapidjson::Document document(rapidjson::kObjectType);
					snake = std::make_unique<PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{
						                                      static_cast<float>(config.gridWidth) - 1,
						                                      static_cast<float>(config.gridHeight) / 2
					                                      });
					rapidjson::Value value = snake->toJson(document.GetAllocator());
					document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
					document.AddMember("state", rapidjson::StringRef("start"), document.GetAllocator());
					document.AddMember("snake", value, document.GetAllocator());
					document.AddMember("score", score, document.GetAllocator());
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					document.Accept(writer);
					auto data = new std::string(buffer.GetString());
					client.send(*data, [data](const std::error_code ec, size_t t)
					{
						delete data;
					});

					client.receive([&](const std::string& data)
					{
						document = rapidjson::Document();
						document.Parse(data.c_str());
						if (document["dataType"] == "snake")
						{
							remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
							remoteSnake->config = config;
							remoteScore = document["score"].GetInt();
							canDrawRemoteSnake = true;
						}
						else if (document["dataType"] == "cells")
						{
							const int x = document["x"].GetInt();
							const int y = document["y"].GetInt();
							const auto& cell = std::make_shared<Cell>(Cell::fromJson(document["cell"]));
							cells[y][x]->type = cell->type;
						}
						else if (document["dataType"] == "state")
						{
							if (document["state"] == "playing")
							{
								gameState = GameState::Playing;
								syncState();
							}
							else if (document["state"] == "pause")
							{
								gameState = GameState::Paused;
							}
							else if (document["state"] == "menu")
							{
								gameState = GameState::Menu;
							}
							else
							{
								gameState = GameState::Start;
							}
						}
					});
					context.run();
				}).detach();
			}
		}
	}
	else if (gameState == GameState::Playing)
	{
		remoteSnake->draw();
		// draw score
		char scoreText[30];
		sprintf_s(scoreText, "RightPlayer Score: %d", remoteScore);
		DrawText(scoreText, 10, 30, 20, BLACK);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.isClicked(GetMousePosition()))
		{
			snake->direction = {0, 0};
		}
	}
	else if (gameState == GameState::Paused)
	{
		char scoreText[30];
		sprintf_s(scoreText, "RightPlayer Score: %d", remoteScore);
		DrawText(scoreText, 10, 30, 20, BLACK);
		if (!remoteSnake->isDead)
		{
			remoteSnake->draw();
		}
		if (isHost)
		{
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
			{
				rapidjson::Document document(rapidjson::kObjectType);
				document.AddMember("dataType", rapidjson::StringRef("state"), document.GetAllocator());
				document.AddMember("state", rapidjson::StringRef("menu"), document.GetAllocator());
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				document.Accept(writer);
				auto data = new std::string(buffer.GetString());
				server.send(*data, [data](const std::error_code ec, size_t t)
				{
					delete data;
				});
				context.stop();
				isContextRun = false;
			}
		}
	}
	else if (gameState == GameState::GameOver)
	{
		char scoreText[30];
		sprintf_s(scoreText, "RemotePlayer Score: %d", remoteScore);
		DrawText(scoreText, 10, 30, 20, BLACK);
		if (!remoteSnake->isDead)
		{
			remoteSnake->draw();
		}
		if (isHost)
		{
			restartButton.bounds.width = 200;
			restartButton.text = "Restart";
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
			{
				rapidjson::Document document(rapidjson::kObjectType);
				document.AddMember("dataType", rapidjson::StringRef("state"), document.GetAllocator());
				document.AddMember("state", rapidjson::StringRef("menu"), document.GetAllocator());
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				document.Accept(writer);
				auto data = new std::string(buffer.GetString());
				if (isHost)
				{
					server.send(*data, [data](const std::error_code ec, size_t t)
					{
						delete data;
					});
				}
				context.stop();
				isContextRun = false;
			}
		}
		else
		{
			restartButton.bounds.width = 0;
			restartButton.text = "";
		}
	}
	GameBase::draw();
}
