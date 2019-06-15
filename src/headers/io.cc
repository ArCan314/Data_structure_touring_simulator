#ifndef SRC_IO_CC
#define SRC_IO_CC

#pragma execution_character_set("utf-8")

#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <exception>

#include "io.h"
#include "city_graph.h"
#include "traveller.h"
#include "id_map.h"
#include "user_type.h"
#include "path.h"
#include "time_format.h"
#include "log.h"

extern const std::string name_path;
extern const int kCityNum;

#if defined(_WIN32) || (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__MINGW32__) || defined(__MINGW64__)
#include <Windows.h>
#endif // _WIN32

int Welcome(Traveller &traveller)
{
    std::cout << "|----------------------------------------------|" << std::endl;
    std::cout << std::endl;
    std::cout << "|                                              |" << std::endl;
    std::cout << std::endl;
    std::cout << "|                   Welcome!                   |" << std::endl;
    std::cout << std::endl;
    std::cout << "|                                              |" << std::endl;
    std::cout << std::endl;
    std::cout << "|----------------------------------------------|" << std::endl;
    std::cout << std::endl;
    std::cout << "选择注册还是已有账号登陆？" << std::endl;
    std::cout << "注册：S           登陆：L" << std::endl;
    std::string option_str;
    char option;
    while (true)
    {
        std::getline(std::cin, option_str);
        std::cin.clear();
        option = FindFirstAlpha(option_str);

        if (option == 'S' || option == 's')
        {
            std::cout << "请输入您想注册的账号：";
            std::string account_name;
            getline(std::cin, account_name);
            std::cin.clear();

            while (!IsValidName(account_name))
            {
                ErrorMsg("非法的用户名，请重新输入：");
                getline(std::cin, account_name);
                std::cin.clear();
            }

            while (AccountCheck(account_name) != -1)
            {
                std::cout << "该账号已被注册，请重新输入：";
                getline(std::cin, account_name);
                std::cin.clear();

                while (!IsValidName(account_name))
                {
                    ErrorMsg("非法的用户名，请重新输入：");
                    getline(std::cin, account_name);
                    std::cin.clear();
                }
            }
            if (AddAccount(account_name))
                Log::LogWrite(std::string("注册账号") + account_name);
            else
                Log::LogWrite(std::string("注册账号") + account_name + "失败");
            traveller.set_id(account_name);
            std::cout << "您已注册账号：" << account_name << std::endl;
            return -1;
        }
        else if (option == 'l' || option == 'L')
        {
            std::cout << "已注册的账号列表如下：" << std::endl;
            PrintNameList();

            std::cout << "请输入您的账号：";
            std::string account_name;
            getline(std::cin, account_name);
            std::cin.clear();

            while (!IsValidName(account_name))
            {
                ErrorMsg("非法的用户名,请重新输入");
                getline(std::cin, account_name);
                std::cin.clear();
            }

            while (AccountCheck(account_name) == -1)
            {
                std::string option_str;
                char option;

                std::cout << "该账号不存在，是否需要注册该账号?[Y/N]" << std::endl;

                std::getline(std::cin, option_str);
                std::cin.clear();
                option = FindFirstAlpha(option_str);
                while (option != 'Y' && option != 'N' && option != 'n' && option != 'y')
                {
                    std::cout << "无效的选项，请重新输入" << std::endl;
                    std::getline(std::cin, option_str);
                    std::cin.clear();
                    option = FindFirstAlpha(option_str);
                }
                if (option == 'Y' || option == 'y')
                {
                    AddAccount(account_name);
                    traveller.set_id(account_name);
                    return -1;
                }
                else if (option == 'N' || option == 'n')
                {
                    std::cout << "请重新输入您的账号：";
                    getline(std::cin, account_name);
                    std::cin.clear();

                    while (!IsValidName(account_name))
                    {
                        ErrorMsg("非法的用户名，请重新输入：");
                        getline(std::cin, account_name);
                        std::cin.clear();
                    }
                }
            }
            Log::LogWrite(std::string("登陆账号") + account_name);
            return AccountCheck(account_name);
        }
        else if (option == 'q' || option == 'Q')
        {
            Log::LogWrite("程序退出");
            exit(0);
        }
        else
        {
            std::cout << "无效的选项，请重新输入：";
        }
    }
}

//功能菜单,返回一个操作代码
int Menu(const IDMap &im, Traveller &traveller)
{
    std::cout << std::endl;
    std::cout << "输入对应数字获取功能：" << std::endl
              << "1、预定行程" << std::endl
              << "2、状态查询" << std::endl
              << "3、路线查询" << std::endl
              << "4、模拟旅行" << std::endl
              << "5、设    置" << std::endl
              << "6、退出程序" << std::endl;
    std::string buf;
    char num;
    std::vector<City_id> plan;
    while (std::getline(std::cin, buf))
    {
        if (!buf.size())
        {
            // std::cout << "!!!" << std::endl;
            continue;
        }
        num = FindFirstDigit(buf);
        int operate_code = num - '0';

        if (operate_code == SCHEDULE)
        {
            Log::LogWrite("用户选择预定行程");
            ClearScreen();
            std::cout << "-------------------------------------------------" << std::endl;
            return operate_code;
        }
        else if (operate_code == INQUIRE_STATE)
        {
            Log::LogWrite("用户选择查询状态");
            ClearScreen();
            std::cout << "-------------------------------------------------" << std::endl;
            return operate_code;
        }
        else if (operate_code == INQUIRE_PATH)
        {
            Log::LogWrite("用户选择查询路径");
            ClearScreen();
            std::cout << "-------------------------------------------------" << std::endl;
            return operate_code;
        }
        else if (operate_code == SIMULATE)
        {
            Log::LogWrite("用户选择模拟旅行");
            ClearScreen();
            return operate_code;
        }
        else if (operate_code == SETTINGS)
        {
            Log::LogWrite("用户选择设置");
            ClearScreen();
            return operate_code;
        }
        else if (operate_code == EXIT)
        {
            Log::LogWrite("程序退出");
            return operate_code;
        }
        else
        {
            std::cout << "无效的选项，请重新输入：";
        }
    }
    std::cin.clear();
    return -1;
}

std::vector<City_id> Request(const IDMap &im)
{
    std::vector<City_id> res;
    std::string id;
    City_id temp_id;

    std::cout << "当前支持的城市有：" << std::endl;
    for (int i = 0; i < im.getCityMapSize(); i++)
    {
        std::cout << i + 1 << " : " << im.getCityStr(i) << std::endl;
    }

    std::cout << "请输入您的当前城市(输入数字)：";
    while (true)
    {
        if (!std::cin.good())
            std::cin.clear();
        std::cin >> id;
        if (std::cin.good())
        {
            while (true)
            {
                try
                {
                    temp_id = std::stoi(id) - 1;
                    break;
                }
                catch (const std::exception &e)
                {
                    std::cout << "请输入数字：";
                }
                std::cin >> id;
                std::cin.clear();
            }

            if (temp_id < im.getCityMapSize() && temp_id >= 0)
            {
                std::cout << "您选择的当前城市是：" << im.getCityStr(temp_id) << std::endl;
                res.push_back(temp_id);
                break;
            }
            else
            {
                ErrorMsg("无效的输入，请重新输入");
            }
        }
    }

    Log::LogWrite(std::string("起始城市:") + im.getCityStr(temp_id));
    std::cout << "请输入您希望经过的城市(以q结束)：";
    while (true)
    {
        if (!std::cin.good())
            std::cin.clear();
        std::cin >> id;
        if (std::cin.good())
        {
            while (true)
            {
                try
                {
                    if (id == "q")
                        break;

                    temp_id = std::stoi(id) - 1;
                    break;
                }
                catch (const std::exception &e)
                {
                    std::cout << "请输入数字或q：";
                }
                std::cin >> id;
                std::cin.clear();
            }

            if (id == "q")
                break;

            if (temp_id >= im.getCityMapSize() || temp_id < 0)
                ErrorMsg("无效的城市，请重新输入");
            else if (temp_id == res.front())
                ErrorMsg("与起点重复，请重新输入");
            else if (IsInplan(res, temp_id))
                ErrorMsg("已存在于计划中，请重新输入");
            else
            {
                res.push_back(temp_id);
            }
        }
    }
    if (res.size() == 1)
    {
        Log::LogWrite("不经过任何城市");
        std::cout << "您选择不经过任何城市" << std::endl;
    }
    else
    {
        std::cout << "您选择经过的城市是：";
        std::string pass;
        for (auto i = ++res.begin(); i != res.end(); ++i)
            pass += im.getCityStr(*i) + " ";
        Log::LogWrite(std::string("经过以下城市:") + pass);
        std::for_each(++res.begin(), res.end(), [&](City_id city_id) { std::cout << im.getCityStr(city_id) << " "; });
        std::cout << std::endl;
    }
    std::cout << "请输入您的目的城市：";
    while (true)
    {
        if (!std::cin.good())
            std::cin.clear();
        std::cin >> id;
        if (std::cin.good())
        {
            while (true)
            {
                try
                {
                    temp_id = std::stoi(id) - 1;
                    break;
                }
                catch (const std::exception &e)
                {
                    std::cout << "请输入数字：";
                }
                std::cin >> id;
                std::cin.clear();
            }

            if (std::find(res.begin(), res.end(), temp_id) != res.end())
            {
                ErrorMsg("重复的城市，请重新输入");
            }
            else if (temp_id >= im.getCityMapSize() || temp_id < 0)
                ErrorMsg("无效的城市，请重新输入");
            else
            {
                std::cout << "您选择的目的城市是：" << im.getCityStr(temp_id) << std::endl;
                res.push_back(temp_id);
                break;
            }
        }
    }
    Log::LogWrite(std::string("目的城市:") + im.getCityStr(temp_id));
    return res;
}

// 打印账户列表
bool PrintNameList()
{
    std::string line_buf;
    std::ifstream fis(name_path);
    if (fis)
    {
        while (std::getline(fis, line_buf))
            std::cout << line_buf << std::endl;
        return true;
    }
    else
        return false;
}

// 根据所提供的traveller添加账号
bool AddAccount(const Traveller &traveller)
{
    bool add_success = traveller.saveData();
    if (add_success)
    {
        Log::LogWrite("添加账号成功");
    }
    else
    {
        Log::LogWrite("添加账号失败");
    }

    return add_success;
}

// 根据用户名添加账号
bool AddAccount(const std::string &account_name)
{
    std::ofstream fos(name_path, std::ofstream::app);
    if (fos)
    {
        Log::LogWrite("添加账号成功");
        fos << account_name << std::endl;
        return true;
    }
    else
    {
        Log::LogWrite("添加账号失败, 文件未打开");
        return false;
    }
}

bool PathConfirm()
{
    std::string option_str;
    char option;
    std::cout << "是否选择该条路线?[Y/N]" << std::endl;
    while (true)
    {
        std::getline(std::cin, option_str);

        if (!option_str.size())
            continue;
        option = FindFirstAlpha(option_str);
        if (option == 'Y' || option == 'y')
        {
            Log::LogWrite("确认选择该路线");
            return true;
        }
        else if (option == 'N' || option == 'n')
        {
            Log::LogWrite("不选择该路线");
            return false;
        }
        else
        {
            ErrorMsg("无效的输入，请重新输入");
            std::cin.clear();
        }
    }
}

Strategy InputStrategy(Time &init_time, Time &limit_time)
{
    std::string strategy_str;
    int strategy;
    std::cout << "输入旅行策略" << std::endl
              << "1. 最少金钱" << std::endl
              << "2. 最少时间" << std::endl
              << "3. 限定时间内最少金钱" << std::endl;
    while (true)
    {
        std::getline(std::cin, strategy_str);
        std::cin.clear();
        if (!strategy_str.size())
            continue;
        strategy = FindFirstDigit(strategy_str) - '0';

        switch (strategy - 1)
        {
        case 0:
            init_time = InputInitTime();
            Log::LogWrite(std::string("起始时间: ") + std::to_string(init_time.get_hour()) + " 旅行策略: 最少价格");
            return LEAST_MONEY;
        case 1:
            init_time = InputInitTime();
            Log::LogWrite(std::string("起始时间: ") + std::to_string(init_time.get_hour()) + " 旅行策略: 最少时间");
            return LEAST_TIME;
        case 2:
            init_time = InputInitTime();
            limit_time = InputLimitTime();
            Log::LogWrite(std::string("起始时间: ") + std::to_string(init_time.get_hour()) + "限定时间: " + std::to_string(init_time.get_length()) + "小时 " + " 旅行策略: 限定时间内最小价格");
            return LIMIT_TIME;
        default:
            ErrorMsg("无效的输入，请重新输入");
            break;
        }
    }
}

// 返回账户名称所在的行数,若账户名称不存在则返回-1
int AccountCheck(const std::string &id)
{
    std::vector<std::string> namelist; // unused parameter
    std::ifstream in_stream(name_path);
    if (in_stream.is_open())
    {
        int cnt = 0;
        std::string temp;
        while (getline(in_stream, temp))
        {
            if (temp == id)
            {
                return cnt;
            }
            cnt++;
        }
    }
    return -1;
}

void PrintTravellerInfo(const CityGraph &graph, const IDMap &id_map, const Time &now, const Traveller &traveller)
{
    auto plan = traveller.get_plan();
    auto path = traveller.get_path();
    auto position = traveller.get_position();

    //std::cout << "用户名: " <<traveller.get_ID() << std::endl;
    if (traveller.get_state() == STAY)
    {
        if (position == -2)
            std::cout << "当前无出行计划" << std::endl;
        else if (position == -1)
        {
            std::cout << std::endl;
            std::cout << "您的始发地是：" << id_map.getCityStr(plan.front()) << std::endl;
            std::cout << "您的目的地是：" << id_map.getCityStr(plan.back()) << std::endl;

            if (plan.size() > 2)
            {
                std::cout << "您的途经城市有：";
                for (int i = 1; i < plan.size() - 1; i++)
                {
                    std::cout << id_map.getCityStr(plan.at(i)) << " ";
                }
                std::cout << std::endl;
            }
            PrintPath(graph, id_map, path);
        }
        else
        {
            PrintPath(graph, id_map, path, position);
            std::cout << "当前在：" << id_map.getCityStr(path.getNode(position).former_city) << std::endl;
            std::cout << "离出发还有：" << traveller.get_left_hour() << "h" << std::endl;
        }
    }
    else
    {
        PrintPath(graph, id_map, path, position);
        std::cout << "距目的地 " << id_map.getCityStr(path.getNode(position).current_city) << " 还有：" << traveller.get_left_hour() << "h" << std::endl;
    }
}

std::ostream &PrintPath(const CityGraph &graph, const IDMap &id_map, const Path &path, std::ostream &os)
{
    return PrintPath(graph, id_map, path, 0, true);
}

std::ostream &PrintPath(const CityGraph &graph, const IDMap &id_map, const Path &path, const int index, bool showtotal, std::ostream &os)
{
    std::string comp("三个字");
    std::string wrap[] = {"\t\t", "\t"};
    //std::cout << "为您定制的路线为：" << std::endl;
    std::cout << "始发地"
              << "\t\t"
              << "目的地"
              << "\t\t"
              << "方式" << '\t'
              << "出发时间" << '\t'
              << "到达时间" << '\t'
              << "价格" << '\t'
              << "完成" << '\t' << std::endl;
    for (int path_node = 0; path_node < index; path_node++)
    {
        int i = path.getNode(path_node).former_city;
        int j = path.getNode(path_node).current_city;
        int k = path.getNode(path_node).kth_way;
        Route route = graph.get_route(i, j, k);
        auto former_city_str = id_map.getCityStr(i);
        auto current_city_str = id_map.getCityStr(j);
        std::cout << former_city_str << wrap[former_city_str.size() > comp.size()]
                  << current_city_str << wrap[current_city_str.size() > comp.size()]
                  << id_map.getTransStr(route.transport_type) << '\t'
                  << RouteShow(route.start_time, route.end_time)
                  << route.price << '\t'
                  << "O" << '\t' << std::endl;
    }

    for (int path_node = index; path_node < path.get_len(); path_node++)
    {
        int i = path.getNode(path_node).former_city;
        int j = path.getNode(path_node).current_city;
        int k = path.getNode(path_node).kth_way;
        Route route = graph.get_route(i, j, k);
        auto former_city_str = id_map.getCityStr(i);
        auto current_city_str = id_map.getCityStr(j);
        std::cout << former_city_str << wrap[former_city_str.size() > comp.size()]
                  << current_city_str << wrap[current_city_str.size() > comp.size()]
                  << id_map.getTransStr(route.transport_type) << '\t'
                  << RouteShow(route.start_time, route.end_time)
                  << route.price << '\t'
                  << "X" << '\t' << std::endl;
    }
    if (showtotal)
    {
        std::cout << "总价格花费：" << path.get_total_price() << std::endl;
        std::cout << "总时间花费：" << path.get_total_time().get_length() << "h" << std::endl;
    }
    return os;
}

double getSimulateSpeed()
{
    std::cout << "请输入两次模拟的间隔时间(在0.5秒和10秒之间)：";
    std::string line;
    double sleep_sec;
    std::exception excpt;

    while (1)
    {
        try
        {
            getline(std::cin, line);
            std::cin.clear();
            if (!line.size())
                continue;
            else
            {
                sleep_sec = std::stod(line);
                if (sleep_sec < 0.5 || sleep_sec > 10)
                    throw excpt;
                break;
            }
        }
        catch (std::exception excpt)
        {
            std::cout << "输入有误，请重新输入" << std::endl;
        }
    }
    Log::LogWrite(std::string("设置模拟时间间隔为: ") + std::to_string(sleep_sec) + "秒");
    return 1000 * sleep_sec;
}

eSettings SettingsMenu()
{
    std::cout << "输入对应数字获取功能：" << std::endl
              << "1、改变模拟速度" << std::endl
              << "2、改变控制台字体大小(Windows Only)" << std::endl;
    std::string line;
    int option;
    while (1)
    {
        getline(std::cin, line);
        std::cin.clear();

        if (!line.size())
            continue;

        option = FindFirstDigit(line);

        if (option == '1')
        {
            Log::LogWrite("设置模拟速度");
            return SIMULATION_SPEED;
        }
        else if (option == '2')
        {
            Log::LogWrite("设置字体大小");
            return CONSOLE_FONT_SIZE;
        }
        else
        {
            std::cout << "无效的输入，请重新输入" << std::endl;
        }
    }
}

bool SetConsoleFontSize()
{
#if defined(_WIN32) || (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__MINGW32__) || defined(__MINGW64__)
    static HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    static CONSOLE_FONT_INFOEX cfiStdOut;
    SHORT sNewX, sNewY;
    cfiStdOut.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(hStdOut, FALSE, &cfiStdOut);
    GetConsoleFontSize(hStdOut, cfiStdOut.nFont);

    // std::cout << cfiStdOut.FaceName << " "
    //           << "X: " << cfiStdOut.dwFontSize.X << " Y: " << cfiStdOut.dwFontSize.Y << std::endl;
    std::cout << "当前字体大小: " << cfiStdOut.dwFontSize.Y << std::endl;
    std::cout << "请输入新的字体大小: ";
    while (1)
    {
        std::cin >> sNewY;
        std::cin.clear();
        if (sNewY < 1)
        {
            std::cout << "输入有误，请重新输入" << std::endl;
        }
        else
            break;
    }
    // cfiStdOut.dwFontSize.X = sNewX;
    cfiStdOut.dwFontSize.Y = sNewY;

    BOOL bSetFontSizeSuccess = SetCurrentConsoleFontEx(hStdOut, FALSE, &cfiStdOut);
    if (bSetFontSizeSuccess)
        Log::LogWrite(std::string("设置字体大小成功, 字体大小为: ") + std::to_string(sNewY));
    else
        Log::LogWrite("字体大小设置失败");
    return true;

#else
    Log::LogWrite("字体大小设置失败, 非Windows系统");
    std::cout << "该设置目前仅支持Windows系统" << std::endl;
    return false;
#endif
}

// inline bool IsValidName(const std::string &name_str)
// {
//     for (auto i = name_str.begin(); i != name_str.end(); ++i)
//         if (!std::isalnum(*i))
//             return false;
//     return true;
// }

void ClearScreen()
{
#if defined(_WIN32) || (defined(__CYGWIN__) && !defined(_WIN32)) || defined(__MINGW32__) || defined(__MINGW64__)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = {0, 0};

    std::cout.flush();

    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

    DWORD written;

    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

    SetConsoleCursorPosition(hOut, topLeft);
#elif defined(__linux__)
    std::cout << "\x1B[2J\x1B[H";
#endif
}

void PrintRoutes(const CityGraph &graph, const IDMap &id_map)
{
    std::string id;
    City_id start_city;
    City_id target_city;
    std::cout << "输入两个城市，获取两城市间的所有路线" << std::endl;
    for (int i = 0; i < id_map.getCityMapSize(); i++)
    {
        std::cout << i + 1 << " : " << id_map.getCityStr(i) << std::endl;
    }
    std::cout << "请输入始发城市：";
    while (1)
    {
        if (!std::cin.good())
            std::cin.clear();
        std::cin >> id;
        if (std::cin.good())
        {
            start_city = std::stoi(id) - 1;
            if (start_city < id_map.getCityMapSize() && start_city >= 0)
            {
                std::cout << "您选择的始发城市是：" << id_map.getCityStr(start_city) << std::endl;
                break;
            }
            else
            {
                ErrorMsg("无效的输入");
                std::cout << "请输入始发城市：";
            }
        }
    }
    std::cout << "请输入目的城市：";
    while (1)
    {
        if (!std::cin.good())
            std::cin.clear();
        std::cin >> id;
        if (std::cin.good())
        {
            target_city = std::stoi(id) - 1;
            if (target_city < id_map.getCityMapSize() && target_city >= 0)
            {
                std::cout << "您选择的目的城市是：" << id_map.getCityStr(target_city) << std::endl;
                break;
            }
            else
            {
                ErrorMsg("无效的输入");
                std::cout << "请输入始发城市：";
            }
        }
    }
    Log::LogWrite(std::string("获取从 ") + id_map.getCityStr(start_city) + " 到 " + id_map.getCityStr(target_city) + " 的路线");
    int size = graph.get_size(start_city, target_city);
    if (!size)
    {
        std::cout << "两城市间无路线" << std::endl;
    }
    else
    {
        std::string comp("三个字");
        std::string wrap[] = {"\t\t", "\t"};
        std::cout << "序号" << '\t'
                  << "始发地"
                  << "\t\t"
                  << "目的地"
                  << "\t\t"
                  << "方式" << '\t'
                  << "出发时间" << '\t'
                  << "到达时间" << '\t'
                  << "价格" << '\t' << std::endl;

        for (int k = 0; k < size; k++)
        {
            Route route = graph.get_route(start_city, target_city, k);

            std::string cityi = id_map.getCityStr(start_city);
            std::string cityj = id_map.getCityStr(target_city);
            std::string method = id_map.getTransStr(route.transport_type);

            std::cout << k + 1 << '\t'
                      << cityi << wrap[cityi.size() > comp.size()]
                      << cityj << wrap[cityj.size() > comp.size()]
                      << method << '\t';
            std::cout << RouteShow(route.start_time, route.end_time);
            std::cout << route.price << '\t' << std::endl;
        }
    }
}

Time InputLimitTime()
{
    std::string info;
    int day;
    int hour;
	std::size_t digit;

    while (1)
    {
        std::cout << "输入您希望第几天内到达(1代表当天)：";
        if (!std::cin.good())
            std::cin.clear();

        std::cin >> info;
        if (std::cin.good())
        {
			for(digit = 0; digit < info.length(); digit++)
				if (!std::isdigit(info.at(digit)))
					break;

			if (digit == info.length())
				day = std::stoi(info);
			else
			{
				ErrorMsg("无效的输入，请重新输入");
				continue;
			}

            if (day >= 1)
            {
                std::cout << "第" << day << "天到达" << std::endl;
                break;
            }
            else
            {
                ErrorMsg("无效的输入，请重新输入");
            }
        }
    }

    while (1)
    {
        std::cout << "输入您希望到达时刻(输入小时数)：";
        if (!std::cin.good())
            std::cin.clear();

        std::cin >> info;
        if (std::cin.good())
        {
			for (digit = 0; digit < info.length(); digit++)
				if (!std::isdigit(info.at(digit)))
					break;
           
			if (digit == info.length())
				hour = std::stoi(info);
			else
			{
				ErrorMsg("无效的输入，请重新输入");
				continue;
			}

            if (hour >= 0 && hour < 24)
            {

                std::cout << hour << "点前" << std::endl;
                break;
            }
            else
            {
                ErrorMsg("无效的输入，请重新输入");
            }
        }
    }
    return Time(day, hour);
}

Time InputInitTime()
{
    std::string info;
    int hour;
	std::size_t digit;

    while (1)
    {
        std::cout << "输入预计出发时间开始查找(输入小时数)：";
        if (!std::cin.good())
            std::cin.clear();

        std::cin >> info;
        if (std::cin.good())
        {
			for (digit = 0; digit < info.length(); digit++)
				if (!std::isdigit(info.at(digit)))
					break;

			if (digit == info.length())
				hour = std::stoi(info);
			else
			{
				ErrorMsg("无效的输入，请重新输入");
				continue;
			}

            if (hour >= 0 && hour < 24)
            {
				
                std::cout << "输入预计出发时间为：" << hour << ":00" << std::endl;
                break;
            }
            else
            {
                ErrorMsg("无效的输入，请重新输入");
            }
        }
    }
    return Time(1, hour);
}

#endif // SRC_IO_CC
