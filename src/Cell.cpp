#include "../include/Cell.h"

#include <locale>
#include <memory>
#include <thread>

WeightedCell::WeightedCell(const CellType type, const int weight): type(type), weight(weight)
{
}

Cell::Cell(const GameConfig& config): type(CellType::Blank), position(), config(config)
{
}

Cell::Cell(const CellType type, const Vector2& position, const GameConfig& config): type(type),
    position(position),
    config(config)
{
    // std::thread([this]()
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    //     this->type = CellType::Rot;
    // }).detach();
}


void Cell::draw() const
{
    Color color = WHITE;
    switch (type)
    {
    case CellType::Blank:
        return;
    case CellType::Edible:
        color = GREEN;
        break;
    case CellType::Rot:
        color = DARKGREEN;
        break;
    case CellType::Die:
        color = RED;
        break;
    case CellType::Wall:
        color = BLACK;
        break;
    }
    DrawRectangle(position.x * config.tileSize, position.y * config.tileSize, config.tileSize, config.tileSize, color);
}

CellType Cell::randomType(const std::vector<WeightedCell>& weightedCells)
{
    int totalWeight = 0;
    for (const auto& cell : weightedCells)
    {
        totalWeight += cell.weight;
    }
    // 生成随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, totalWeight);
    const int randomNumber = dist(gen);

    int cumulativeWeight = 0;
    for (const auto& cell : weightedCells)
    {
        cumulativeWeight += cell.weight;
        if (randomNumber <= cumulativeWeight)
        {
            return cell.type;
        }
    }
    return CellType::Blank;
}

void Cell::reductionCounter()
{
    if (counter <= 0) return;
    counter--;
}

void Cell::resurgence()
{
    if (counter > 0)
    {
        type = CellType::Edible;
    }
    else
    {
        type = CellType::Rot;
    }
}

rapidjson::Value Cell::toJson(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value cellValue(rapidjson::kObjectType);

    // 添加类型字段
    rapidjson::Value typeValue;
    typeValue.SetInt(static_cast<int>(type));
    cellValue.AddMember("type", typeValue, allocator);

    // 添加位置字段
    rapidjson::Value positionValue(rapidjson::kObjectType);
    positionValue.AddMember("x", position.x, allocator);
    positionValue.AddMember("y", position.y, allocator);
    cellValue.AddMember("position", positionValue, allocator);

    return cellValue;
}

rapidjson::Value Cell::toJsons(const std::vector<std::vector<std::shared_ptr<Cell>>>& grid,
    rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value gridValue(rapidjson::kArrayType);

    // 遍历二维 vector<Cell>，将每个 Cell 对象序列化为 JSON 对象，并添加到 JSON 数组中
    for (const auto& row : grid) {
        rapidjson::Value rowValue(rapidjson::kArrayType);
        for (const auto& cell : row) {
            rapidjson::Value cellValue = cell->toJson(allocator);
            rowValue.PushBack(cellValue, allocator);
        }
        gridValue.PushBack(rowValue, allocator);
    }

    return gridValue;
}


std::vector<std::vector<std::shared_ptr<Cell>>> Cell::fromJsons(const rapidjson::Value& jsonValue)
{
    std::vector<std::vector<std::shared_ptr<Cell>>> grid;

    if (jsonValue.IsArray()) {
        for (const auto& row : jsonValue.GetArray()) {
            std::vector<std::shared_ptr<Cell>> rowCells;
            for (const auto& cellJson : row.GetArray()) {
                Cell cell = fromJson(cellJson);
                rowCells.push_back(std::make_shared<Cell>(cell));
            }
            grid.push_back(rowCells);
        }
    }

    return grid;
}

Cell Cell::fromJson(const rapidjson::Value& jsonValue)
{
    Cell cell;

    // 解析类型字段
    const auto& typeValue = jsonValue["type"];
    if (!typeValue.IsNull() && typeValue.IsInt()) {
        cell.type = static_cast<CellType>(typeValue.GetInt());
    } else {
        // 如果类型字段不存在或者不是整数，设为默认值
        cell.type = CellType::Blank;
    }

    // 解析位置字段
    const auto& positionObj = jsonValue["position"];
    if (positionObj.IsObject()) {
        const auto& xValue = positionObj["x"];
        const auto& yValue = positionObj["y"];
        if (xValue.IsFloat() && yValue.IsFloat()) {
            cell.position.x = xValue.GetFloat();
            cell.position.y = yValue.GetFloat();
        } else {
            // 如果位置字段中的 x 或者 y 不是浮点数，设为默认值
            cell.position = { 0.0f, 0.0f };
        }
    } else {
        // 如果位置字段不存在或者不是对象，设为默认值
        cell.position = { 0.0f, 0.0f };
    }

    return cell;
}
