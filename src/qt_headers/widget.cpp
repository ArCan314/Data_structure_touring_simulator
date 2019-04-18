﻿#ifndef SRC_WIDGET
#define SRC_WIDGET
#pragma execution_character_set("utf-8")
#include <string>
#include <sstream>

#include "widget.h"
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>

Widget::Widget(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->Path_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_LogInButton_released() //登陆
{
    QString account_name = ui->lineEdit->text();

    if (!account_name.size())
    {
        QMessageBox::warning(this, "Warning!", "输入为空", QMessageBox::Ok);
        return;
    }
    else if (!IsValidName(account_name.toStdString()))
    {
        QMessageBox::warning(this, "Warning!", "非法的用户名,请重新输入", QMessageBox::Ok);
        return;
    }

    int account_check = AccountCheck(account_name.toStdString());

    if (account_check != -1)
    {
        traveller_widget.LoadData(account_check, citygraph_widget);
        ui->stackedWidget->setCurrentWidget(ui->MainPage);
    }
    else
    {
        QMessageBox::StandardButton sB = QMessageBox::question(this, "", "该账号不存在，是否注册:" + account_name, QMessageBox::Yes | QMessageBox::No);

        if (sB == QMessageBox::Yes)
        {
            AddAccount(account_name.toStdString()); // ?
            traveller_widget.set_id(account_name.toStdString());
            ui->stackedWidget->setCurrentWidget(ui->MainPage);
        }
    }
}

void Widget::on_SignUpButton_released() //注册
{
    QString account_name = ui->lineEdit->text();

    if (!account_name.size())
    {
        QMessageBox::warning(this, tr("Warning!"), tr("输入为空"), QMessageBox::Ok);
        return;
    }
    else if (!IsValidName(account_name.toStdString()))
    {
        QMessageBox::warning(this, tr("Warning!"), "非法的用户名,请重新输入", QMessageBox::Ok);
        return;
    }

    int account_check = AccountCheck(account_name.toStdString());

    if (account_check != -1)
    {
        QMessageBox::warning(this, tr("Warning!"), tr("该账号已被注册，请重新输入"), QMessageBox::Ok);
        return;
    }
    else
    {
        AddAccount(account_name.toStdString()); //?
        traveller_widget.set_id(account_name.toStdString());
        ui->stackedWidget->setCurrentWidget(ui->MainPage);
    }
}

void Widget::on_OrderPageButton_released() // 预定行程
{
    ui->stackedWidget->setCurrentWidget(ui->OrderPage);
}

void Widget::on_StatePageButton_released() // 状态查询
{
    ui->stackedWidget->setCurrentWidget(ui->StatePage);
}

void Widget::on_QueryPathPageButton_released() // 路线查询
{
    ui->stackedWidget->setCurrentWidget(ui->QueryPage);
    ui->Path_tableWidget->clearContents();
}

void Widget::on_SimulationPageButton_released() //开始模拟
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void Widget::on_StatePageToMenuButton_released() // 从状态页面回到主菜单
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void Widget::on_OrderPageToMenuButton_released() // 从预定页面返回到主菜单
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void Widget::on_QueryPageToMenuButton_released() // 从查询路线页面返回到主菜单
{
    ui->stackedWidget->setCurrentWidget(ui->MainPage);
}

void Widget::on_QueryPathButton_released()
{
    City_id start_city = ui->origin_comboBox->currentIndex();
    City_id target_city = ui->target_comboBox->currentIndex();
    ui->Path_tableWidget->clearContents();

    if (start_city == target_city)
    {
        QMessageBox::warning(this, tr("Warning!"), tr("始发地不能与目的地一样"), QMessageBox::Ok);
        return;
    }

    int size = citygraph_widget.Getsize(start_city, target_city);
    if (!size)
    {
        QMessageBox::warning(this, tr("Warning!"), tr("两城市间无路线"), QMessageBox::Ok);
    }
    else
    {
        auto &path_table = ui->Path_tableWidget;
        path_table->setRowCount(size);

        QString start_city_qstr, target_city_qstr, transport_type_qstr, start_time_qstr, end_time_qstr, price_qstr;
        std::string start_time_str, end_time_str;
        std::stringstream ss;

        start_city_qstr = QString::fromStdString(idmap_widget.GetCityStr(start_city));
        target_city_qstr = QString::fromStdString(idmap_widget.GetCityStr(target_city));

        std::string temp;
        for (int i = 0; i < size; i++)
        {
            Route r = citygraph_widget.GetRoute(start_city, target_city, i);

            transport_type_qstr = QString::fromStdString(idmap_widget.GetTransStr(r.transport_type));

            ss.str(RouteShow(r.start_time, r.end_time));
            ss.clear();
            ss >> start_time_str >> end_time_str;
            start_time_qstr = QString::fromStdString(start_time_str);
            end_time_qstr = QString::fromStdString(end_time_str);

            price_qstr = QString::fromStdString(std::to_string(r.price));
            qDebug() << start_city_qstr << " " << target_city_qstr << " " <<
                        transport_type_qstr << " " << start_time_qstr << " " <<
                        end_time_qstr << " " << price_qstr << endl;
            QTableWidgetItem *id_cell = new QTableWidgetItem(tr(std::to_string(i + 1).c_str()));
            QTableWidgetItem *start_city_cell = new QTableWidgetItem(start_city_qstr);
            QTableWidgetItem *target_city_cell = new QTableWidgetItem(target_city_qstr);
            QTableWidgetItem *transport_type_cell = new QTableWidgetItem(transport_type_qstr);
            QTableWidgetItem *start_time_cell = new QTableWidgetItem(start_time_qstr);
            QTableWidgetItem *end_time_cell = new QTableWidgetItem(end_time_qstr);
            QTableWidgetItem *price_cell = new QTableWidgetItem(price_qstr);

            path_table->setItem(i, 0, id_cell);
            path_table->setItem(i, 1, start_city_cell);
            path_table->setItem(i, 2, target_city_cell);
            path_table->setItem(i, 3, transport_type_cell);
            path_table->setItem(i, 4, start_time_cell);
            path_table->setItem(i, 5, end_time_cell);
            path_table->setItem(i, 6, price_cell);
        }
    }
}

#endif // SRC_WIDGET
