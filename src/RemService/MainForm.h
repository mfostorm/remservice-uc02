#pragma once
#include "OrderForm.h"
#include "DiagnosticForm.h"
#include "WarehouseForm.h"
#include "AdminForm.h"
#include "ReportForm.h"

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Главная форма — рабочее место приёмщика.
    /// Содержит панель ключевых показателей и реестр заказ-нарядов.
    /// </summary>
    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(String^ role, String^ login)
        {
            this->userRole = role;
            this->userLogin = login;
            InitializeComponent();
            LoadKpi();
            LoadOrders();
            ApplyRolePermissions();
        }

    protected:
        ~MainForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        String^ userRole;
        String^ userLogin;

        MenuStrip^ menu;
        Panel^ pnlKpi;
        DataGridView^ dgvOrders;
        StatusStrip^ status;
        ToolStripStatusLabel^ statusLabel;
        Button^ btnNewOrder;
        Button^ btnDiagnostic;
        Button^ btnWarehouse;
        Button^ btnReports;
        Button^ btnAdmin;
        Label^ lblGridTitle;

        Panel^ MakeKpiTile(int x, String^ value, String^ caption, Color back, Color border)
        {
            Panel^ tile = gcnew Panel();
            tile->Location = System::Drawing::Point(x, 8);
            tile->Size = System::Drawing::Size(168, 78);
            tile->BackColor = back;
            tile->BorderStyle = BorderStyle::FixedSingle;

            Label^ lv = gcnew Label();
            lv->Text = value;
            lv->Font = gcnew System::Drawing::Font(L"Segoe UI", 20, FontStyle::Bold);
            lv->ForeColor = Color::FromArgb(36, 48, 63);
            lv->TextAlign = ContentAlignment::MiddleCenter;
            lv->Dock = DockStyle::Top;
            lv->Height = 42;

            Label^ lc = gcnew Label();
            lc->Text = caption;
            lc->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.25f);
            lc->ForeColor = Color::FromArgb(74, 86, 101);
            lc->TextAlign = ContentAlignment::TopCenter;
            lc->Dock = DockStyle::Fill;

            tile->Controls->Add(lc);
            tile->Controls->Add(lv);
            return tile;
        }

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->menu = gcnew MenuStrip();
            this->pnlKpi = gcnew Panel();
            this->dgvOrders = gcnew DataGridView();
            this->status = gcnew StatusStrip();
            this->statusLabel = gcnew ToolStripStatusLabel();
            this->btnNewOrder = gcnew Button();
            this->btnDiagnostic = gcnew Button();
            this->btnWarehouse = gcnew Button();
            this->btnReports = gcnew Button();
            this->btnAdmin = gcnew Button();
            this->lblGridTitle = gcnew Label();
            this->SuspendLayout();

            // Главное меню
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Заказ-наряды"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Диагностика"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Склад"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Клиенты"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Отчёты"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Администрирование"));
            this->menu->Items->Add(gcnew ToolStripMenuItem(L"Справка"));

            // Панель KPI
            this->pnlKpi->Location = System::Drawing::Point(12, 32);
            this->pnlKpi->Size = System::Drawing::Size(956, 94);
            this->pnlKpi->BackColor = Color::Transparent;

            // Кнопки действий
            this->btnNewOrder->Text = L"+  Новый заказ-наряд";
            this->btnNewOrder->Location = System::Drawing::Point(12, 132);
            this->btnNewOrder->Size = System::Drawing::Size(170, 30);
            this->btnNewOrder->BackColor = Color::FromArgb(60, 140, 90);
            this->btnNewOrder->ForeColor = Color::White;
            this->btnNewOrder->FlatStyle = FlatStyle::Flat;
            this->btnNewOrder->Click += gcnew EventHandler(this, &MainForm::btnNewOrder_Click);

            this->btnDiagnostic->Text = L"Диагностика и ремонт";
            this->btnDiagnostic->Location = System::Drawing::Point(190, 132);
            this->btnDiagnostic->Size = System::Drawing::Size(170, 30);
            this->btnDiagnostic->FlatStyle = FlatStyle::Flat;
            this->btnDiagnostic->Click += gcnew EventHandler(this, &MainForm::btnDiagnostic_Click);

            this->btnWarehouse->Text = L"Склад запчастей";
            this->btnWarehouse->Location = System::Drawing::Point(368, 132);
            this->btnWarehouse->Size = System::Drawing::Size(170, 30);
            this->btnWarehouse->FlatStyle = FlatStyle::Flat;
            this->btnWarehouse->Click += gcnew EventHandler(this, &MainForm::btnWarehouse_Click);

            this->btnReports->Text = L"Отчёты и аналитика";
            this->btnReports->Location = System::Drawing::Point(546, 132);
            this->btnReports->Size = System::Drawing::Size(170, 30);
            this->btnReports->FlatStyle = FlatStyle::Flat;
            this->btnReports->Click += gcnew EventHandler(this, &MainForm::btnReports_Click);

            this->btnAdmin->Text = L"Администрирование";
            this->btnAdmin->Location = System::Drawing::Point(724, 132);
            this->btnAdmin->Size = System::Drawing::Size(170, 30);
            this->btnAdmin->FlatStyle = FlatStyle::Flat;
            this->btnAdmin->Click += gcnew EventHandler(this, &MainForm::btnAdmin_Click);

            this->lblGridTitle->Text = L"Заказ-наряды  ·  фильтр: статус «все», период 01.09 – 22.09.2026";
            this->lblGridTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f, FontStyle::Bold);
            this->lblGridTitle->ForeColor = Color::FromArgb(51, 64, 79);
            this->lblGridTitle->Location = System::Drawing::Point(12, 172);
            this->lblGridTitle->AutoSize = true;

            // Таблица заказ-нарядов
            this->dgvOrders->Location = System::Drawing::Point(12, 194);
            this->dgvOrders->Size = System::Drawing::Size(956, 300);
            this->dgvOrders->AllowUserToAddRows = false;
            this->dgvOrders->ReadOnly = true;
            this->dgvOrders->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            this->dgvOrders->BackgroundColor = Color::White;
            this->dgvOrders->BorderStyle = BorderStyle::FixedSingle;
            this->dgvOrders->RowHeadersVisible = false;
            this->dgvOrders->AlternatingRowsDefaultCellStyle->BackColor =
                Color::FromArgb(247, 249, 252);
            this->dgvOrders->ColumnHeadersDefaultCellStyle->BackColor =
                Color::FromArgb(231, 235, 240);
            this->dgvOrders->ColumnHeadersDefaultCellStyle->Font =
                gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            this->dgvOrders->EnableHeadersVisualStyles = false;
            this->dgvOrders->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            // Строка состояния
            this->statusLabel->Text = L"Подключено к RemServiceDB (SQL Server)";
            this->status->Items->Add(this->statusLabel);

            // Форма
            this->ClientSize = System::Drawing::Size(980, 540);
            this->Text = L"РемСервис — рабочее место сотрудника";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->MainMenuStrip = this->menu;
            this->Controls->Add(this->dgvOrders);
            this->Controls->Add(this->lblGridTitle);
            this->Controls->Add(this->btnNewOrder);
            this->Controls->Add(this->btnDiagnostic);
            this->Controls->Add(this->btnWarehouse);
            this->Controls->Add(this->btnReports);
            this->Controls->Add(this->btnAdmin);
            this->Controls->Add(this->pnlKpi);
            this->Controls->Add(this->status);
            this->Controls->Add(this->menu);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        /// <summary>Заполнение панели ключевых показателей.</summary>
        void LoadKpi()
        {
            pnlKpi->Controls->Add(MakeKpiTile(0, L"28", L"Заказ-нарядов в работе",
                Color::FromArgb(218, 232, 252), Color::FromArgb(108, 142, 191)));
            pnlKpi->Controls->Add(MakeKpiTile(180, L"6", L"Ожидают согласования",
                Color::FromArgb(255, 242, 204), Color::FromArgb(214, 182, 86)));
            pnlKpi->Controls->Add(MakeKpiTile(360, L"9", L"Готовы к выдаче",
                Color::FromArgb(213, 232, 212), Color::FromArgb(130, 179, 102)));
            pnlKpi->Controls->Add(MakeKpiTile(540, L"3", L"Просрочено по сроку",
                Color::FromArgb(248, 206, 204), Color::FromArgb(184, 84, 80)));
            pnlKpi->Controls->Add(MakeKpiTile(720, L"412 500 ₽", L"Выручка за месяц",
                Color::FromArgb(225, 213, 231), Color::FromArgb(150, 115, 166)));
        }

        /// <summary>Загрузка реестра заказ-нарядов (демонстрационные данные).</summary>
        void LoadOrders()
        {
            dgvOrders->Columns->Add(L"num", L"№ заказ-наряда");
            dgvOrders->Columns->Add(L"client", L"Клиент");
            dgvOrders->Columns->Add(L"device", L"Техника");
            dgvOrders->Columns->Add(L"fault", L"Неисправность");
            dgvOrders->Columns->Add(L"din", L"Принят");
            dgvOrders->Columns->Add(L"dout", L"Срок");
            dgvOrders->Columns->Add(L"state", L"Статус");
            dgvOrders->Columns->Add(L"sum", L"Сумма, ₽");

            dgvOrders->Rows->Add(L"РН-0241", L"Смирнов А. В.", L"Ноутбук ASUS X515",
                L"Не включается", L"12.09.2026", L"19.09.2026", L"Готов к выдаче", L"7 400");
            dgvOrders->Rows->Add(L"РН-0242", L"Кузнецова Е. И.", L"Смартфон Xiaomi 12",
                L"Разбит дисплей", L"14.09.2026", L"21.09.2026", L"В ремонте", L"11 900");
            dgvOrders->Rows->Add(L"РН-0243", L"ООО «Стройсервис»", L"МФУ HP LaserJet",
                L"Не тянет бумагу", L"15.09.2026", L"22.09.2026", L"Ожидает запчасть", L"3 250");
            dgvOrders->Rows->Add(L"РН-0244", L"Петров И. С.", L"Персональный компьютер",
                L"Перегрев, шум", L"16.09.2026", L"23.09.2026", L"Диагностика", L"—");
            dgvOrders->Rows->Add(L"РН-0245", L"Фёдорова М. А.", L"Пылесос Dyson V8",
                L"Не держит заряд", L"17.09.2026", L"24.09.2026", L"Ожидает согласования", L"5 800");
            dgvOrders->Rows->Add(L"РН-0246", L"Гусев Д. Н.", L"Телевизор LG 43",
                L"Нет изображения", L"18.09.2026", L"26.09.2026", L"Контроль качества", L"9 100");
            dgvOrders->Rows->Add(L"РН-0247", L"Зайцева О. П.", L"Кофемашина DeLonghi",
                L"Течь воды", L"19.09.2026", L"27.09.2026", L"В ремонте", L"6 450");
            dgvOrders->Rows->Add(L"РН-0248", L"Орлов В. В.", L"Ноутбук Lenovo IdeaPad 3",
                L"Замена клавиатуры", L"20.09.2026", L"25.09.2026", L"Принят", L"4 300");

            // Цветовая индикация статусов
            for each (DataGridViewRow ^ row in dgvOrders->Rows)
            {
                String^ st = row->Cells[6]->Value->ToString();
                if (st == L"Готов к выдаче")
                    row->Cells[6]->Style->ForeColor = Color::FromArgb(60, 140, 90);
                else if (st == L"Ожидает запчасть" || st == L"Ожидает согласования")
                    row->Cells[6]->Style->ForeColor = Color::FromArgb(184, 134, 11);
                else
                    row->Cells[6]->Style->ForeColor = Color::FromArgb(46, 109, 164);
            }
            statusLabel->Text = String::Format(
                L"Подключено к RemServiceDB  ·  пользователь: {0} ({1})  ·  записей: {2}",
                userLogin, userRole, dgvOrders->Rows->Count);
        }

        /// <summary>Разграничение доступа к функциям по роли пользователя.</summary>
        void ApplyRolePermissions()
        {
            bool isAdmin = (userRole == L"Администратор");
            bool isChief = (userRole == L"Руководитель") || isAdmin;
            btnAdmin->Enabled = isAdmin;
            btnReports->Enabled = isChief;
            btnWarehouse->Enabled = (userRole == L"Кладовщик") || isAdmin ||
                (userRole == L"Мастер по ремонту");
            this->Text = String::Format(L"РемСервис — рабочее место: {0}", userRole);
        }

        System::Void btnNewOrder_Click(System::Object^ sender, System::EventArgs^ e)
        {
            (gcnew OrderForm())->ShowDialog();
        }
        System::Void btnDiagnostic_Click(System::Object^ sender, System::EventArgs^ e)
        {
            (gcnew DiagnosticForm())->ShowDialog();
        }
        System::Void btnWarehouse_Click(System::Object^ sender, System::EventArgs^ e)
        {
            (gcnew WarehouseForm())->ShowDialog();
        }
        System::Void btnReports_Click(System::Object^ sender, System::EventArgs^ e)
        {
            (gcnew ReportForm())->ShowDialog();
        }
        System::Void btnAdmin_Click(System::Object^ sender, System::EventArgs^ e)
        {
            (gcnew AdminForm())->ShowDialog();
        }
    };
}
