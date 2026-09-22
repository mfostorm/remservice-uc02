#pragma once

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Форма администратора для работы с базой данных склада запасных частей.
    /// Реализует операции INSERT / UPDATE / DELETE над таблицей Parts.
    /// </summary>
    public ref class WarehouseForm : public System::Windows::Forms::Form
    {
    public:
        WarehouseForm(void) { InitializeComponent(); LoadParts(); UpdateSummary(); }
    protected:
        ~WarehouseForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        TextBox^ txtSearch; ComboBox^ cmbCategory;
        Button^ btnFind; Button^ btnReset;
        DataGridView^ dgv;
        Button^ btnAdd; Button^ btnEdit; Button^ btnDelete;
        Button^ btnSupply; Button^ btnExport; Button^ btnRefresh;
        Label^ lblSummary; StatusStrip^ status; ToolStripStatusLabel^ statusLabel;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            Label^ lblSearch = gcnew Label();
            lblSearch->Text = L"Поиск:";
            lblSearch->Location = System::Drawing::Point(12, 16);
            lblSearch->Size = System::Drawing::Size(56, 20);

            txtSearch = gcnew TextBox();
            txtSearch->Location = System::Drawing::Point(72, 13);
            txtSearch->Size = System::Drawing::Size(260, 24);
            txtSearch->Text = L"блок питания";

            Label^ lblCat = gcnew Label();
            lblCat->Text = L"Категория:";
            lblCat->Location = System::Drawing::Point(348, 16);
            lblCat->Size = System::Drawing::Size(80, 20);

            cmbCategory = gcnew ComboBox();
            cmbCategory->Location = System::Drawing::Point(432, 13);
            cmbCategory->Size = System::Drawing::Size(210, 24);
            cmbCategory->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbCategory->Items->AddRange(gcnew cli::array<Object^>(6) {
                L"Все категории", L"Комплектующие", L"Расходные материалы",
                L"Дисплеи", L"Аккумуляторы", L"Бытовая техника" });
            cmbCategory->SelectedIndex = 1;

            btnFind = gcnew Button(); btnFind->Text = L"Найти";
            btnFind->Location = System::Drawing::Point(656, 12);
            btnFind->Size = System::Drawing::Size(110, 27);
            btnFind->BackColor = Color::FromArgb(46, 109, 164);
            btnFind->ForeColor = Color::White; btnFind->FlatStyle = FlatStyle::Flat;
            btnFind->Click += gcnew EventHandler(this, &WarehouseForm::btnFind_Click);

            btnReset = gcnew Button(); btnReset->Text = L"Сбросить";
            btnReset->Location = System::Drawing::Point(774, 12);
            btnReset->Size = System::Drawing::Size(110, 27);
            btnReset->FlatStyle = FlatStyle::Flat;
            btnReset->Click += gcnew EventHandler(this, &WarehouseForm::btnReset_Click);

            dgv = gcnew DataGridView();
            dgv->Location = System::Drawing::Point(12, 50);
            dgv->Size = System::Drawing::Size(872, 300);
            dgv->AllowUserToAddRows = false;
            dgv->RowHeadersVisible = false;
            dgv->BackgroundColor = Color::White;
            dgv->BorderStyle = BorderStyle::FixedSingle;
            dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgv->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(231, 235, 240);
            dgv->ColumnHeadersDefaultCellStyle->Font =
                gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            dgv->EnableHeadersVisualStyles = false;
            dgv->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(247, 249, 252);

            dgv->Columns->Add(L"art", L"Артикул");
            dgv->Columns->Add(L"name", L"Наименование запчасти");
            dgv->Columns->Add(L"cat", L"Категория");
            dgv->Columns->Add(L"price", L"Цена, ₽");
            dgv->Columns->Add(L"stock", L"Остаток");
            dgv->Columns->Add(L"min", L"Мин. остаток");
            dgv->Columns->Add(L"sup", L"Поставщик");
            dgv->Columns->Add(L"state", L"Статус");

            int bx = 12, by = 360, bw = 138, bh = 30;
            btnAdd = MakeBtn(L"Добавить (INSERT)", bx, by, bw, bh,
                Color::FromArgb(60, 140, 90), true);
            btnAdd->Click += gcnew EventHandler(this, &WarehouseForm::btnAdd_Click);
            btnEdit = MakeBtn(L"Изменить (UPDATE)", bx + 146, by, bw, bh,
                Color::FromArgb(46, 109, 164), true);
            btnEdit->Click += gcnew EventHandler(this, &WarehouseForm::btnEdit_Click);
            btnDelete = MakeBtn(L"Удалить (DELETE)", bx + 292, by, bw, bh,
                Color::FromArgb(184, 84, 80), true);
            btnDelete->Click += gcnew EventHandler(this, &WarehouseForm::btnDelete_Click);
            btnSupply = MakeBtn(L"Заказ поставщику", bx + 438, by, bw, bh, Color::White, false);
            btnSupply->Click += gcnew EventHandler(this, &WarehouseForm::btnSupply_Click);
            btnExport = MakeBtn(L"Экспорт в Excel", bx + 584, by, bw, bh, Color::White, false);
            btnExport->Click += gcnew EventHandler(this, &WarehouseForm::btnExport_Click);
            btnRefresh = MakeBtn(L"Обновить", bx + 730, by, bw, bh, Color::White, false);
            btnRefresh->Click += gcnew EventHandler(this, &WarehouseForm::btnRefresh_Click);

            lblSummary = gcnew Label();
            lblSummary->Location = System::Drawing::Point(12, 398);
            lblSummary->Size = System::Drawing::Size(872, 20);
            lblSummary->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f, FontStyle::Bold);
            lblSummary->ForeColor = Color::FromArgb(51, 64, 79);

            status = gcnew StatusStrip();
            statusLabel = gcnew ToolStripStatusLabel();
            statusLabel->Text = L"Роль: Администратор  ·  разрешено изменение таблиц "
                L"Parts, Supplies, PartUsage базы RemServiceDB";
            status->Items->Add(statusLabel);

            this->ClientSize = System::Drawing::Size(896, 452);
            this->Text = L"РемСервис — склад запасных частей (режим администратора)";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(lblSearch); this->Controls->Add(txtSearch);
            this->Controls->Add(lblCat); this->Controls->Add(cmbCategory);
            this->Controls->Add(btnFind); this->Controls->Add(btnReset);
            this->Controls->Add(dgv);
            this->Controls->Add(btnAdd); this->Controls->Add(btnEdit);
            this->Controls->Add(btnDelete); this->Controls->Add(btnSupply);
            this->Controls->Add(btnExport); this->Controls->Add(btnRefresh);
            this->Controls->Add(lblSummary); this->Controls->Add(status);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        Button^ MakeBtn(String^ text, int x, int y, int w, int h, Color back, bool filled)
        {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Location = System::Drawing::Point(x, y);
            b->Size = System::Drawing::Size(w, h);
            b->FlatStyle = FlatStyle::Flat;
            if (filled) { b->BackColor = back; b->ForeColor = Color::White; }
            return b;
        }

        void AddPart(String^ art, String^ name, String^ cat, String^ price,
            int stock, int minStock, String^ sup)
        {
            String^ state = (stock == 0) ? L"Нет в наличии"
                : (stock < minStock ? L"Ниже нормы" : L"В норме");
            int idx = dgv->Rows->Add(art, name, cat, price, stock.ToString(),
                minStock.ToString(), sup, state);
            Color c = (stock == 0) ? Color::FromArgb(184, 84, 80)
                : (stock < minStock ? Color::FromArgb(184, 134, 11)
                    : Color::FromArgb(60, 140, 90));
            dgv->Rows[idx]->Cells[7]->Style->ForeColor = c;
        }

        void LoadParts()
        {
            AddPart(L"BP-450W", L"Блок питания ATX 450 Вт", L"Комплектующие", L"2 900",
                12, 5, L"ООО «Компонент»");
            AddPart(L"BP-650W", L"Блок питания ATX 650 Вт", L"Комплектующие", L"4 100",
                3, 5, L"ООО «Компонент»");
            AddPart(L"TP-GD900", L"Термопаста GD900, 3 г", L"Расходные материалы", L"350",
                48, 20, L"ИП Сорокин");
            AddPart(L"DSP-X515", L"Матрица 15,6\" FHD ASUS", L"Дисплеи", L"8 700",
                0, 2, L"ООО «ТехПартс»");
            AddPart(L"KB-LN-IP3", L"Клавиатура Lenovo IdeaPad 3", L"Клавиатуры", L"1 950",
                6, 3, L"ООО «ТехПартс»");
            AddPart(L"BAT-DV8", L"Аккумулятор Dyson V8", L"Аккумуляторы", L"5 400",
                2, 2, L"ООО «БытСервис»");
            AddPart(L"FUS-HP26", L"Узел термозакрепления HP", L"Печатающая техника", L"6 300",
                4, 2, L"ООО «ОфисТех»");
            AddPart(L"CAP-DL15", L"Прокладка бойлера DeLonghi", L"Бытовая техника", L"480",
                15, 8, L"ООО «БытСервис»");
        }

        void UpdateSummary()
        {
            int below = 0, absent = 0;
            for each (DataGridViewRow ^ r in dgv->Rows)
            {
                String^ s = r->Cells[7]->Value->ToString();
                if (s == L"Ниже нормы") below++;
                if (s == L"Нет в наличии") absent++;
            }
            lblSummary->Text = String::Format(
                L"Позиций в выборке: {0}   ·   ниже нормы: {1}   ·   отсутствует: {2}",
                dgv->Rows->Count, below, absent);
        }

        System::Void btnFind_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(String::Format(
                L"Выполняется запрос:\nSELECT * FROM Parts\nWHERE part_name LIKE '%{0}%'"
                L"\n  AND category = '{1}';", txtSearch->Text, cmbCategory->Text),
                L"Поиск по базе", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnReset_Click(System::Object^ sender, System::EventArgs^ e)
        {
            txtSearch->Clear(); cmbCategory->SelectedIndex = 0;
        }
        System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e)
        {
            AddPart(L"NEW-0001", L"Новая позиция", L"Комплектующие", L"0", 0, 1,
                L"Не указан");
            UpdateSummary();
            MessageBox::Show(L"Выполнен INSERT INTO Parts.\nЗаполните данные новой позиции.",
                L"Добавление записи", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnEdit_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgv->CurrentRow == nullptr) return;
            MessageBox::Show(String::Format(
                L"Выполняется UPDATE Parts\nSET price = …, stock = …\nWHERE article = '{0}';",
                dgv->CurrentRow->Cells[0]->Value), L"Изменение записи",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgv->CurrentRow == nullptr) return;
            if (MessageBox::Show(String::Format(
                L"Удалить позицию «{0}» из базы данных?",
                dgv->CurrentRow->Cells[1]->Value), L"Подтверждение удаления",
                MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
                == System::Windows::Forms::DialogResult::Yes)
            {
                dgv->Rows->Remove(dgv->CurrentRow);
                UpdateSummary();
            }
        }
        System::Void btnSupply_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Сформирован заказ поставщику по позициям ниже "
                L"неснижаемого остатка.", L"Заказ поставщику",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnExport_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Остатки склада выгружены в файл sklad_22_09_2026.xlsx.",
                L"Экспорт", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnRefresh_Click(System::Object^ sender, System::EventArgs^ e)
        {
            UpdateSummary();
            MessageBox::Show(L"Данные обновлены из базы RemServiceDB.",
                L"Обновление", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
