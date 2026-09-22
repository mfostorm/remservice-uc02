#pragma once

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Windows::Forms::DataVisualization::Charting;

    /// <summary>
    /// Форма отчётности и аналитики (рабочее место руководителя).
    /// Соответствует процессу A5.3 «Формирование аналитической отчётности».
    /// </summary>
    public ref class ReportForm : public System::Windows::Forms::Form
    {
    public:
        ReportForm(void) { InitializeComponent(); LoadKpi(); LoadChart(); LoadMasters(); }
    protected:
        ~ReportForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        DateTimePicker^ dtFrom; DateTimePicker^ dtTo;
        ComboBox^ cmbType; Button^ btnBuild; Button^ btnPdf; Button^ btnExcel;
        Panel^ pnlKpi; Chart^ chart; DataGridView^ dgvMasters; GroupBox^ grpConclusion;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            Label^ lblPeriod = gcnew Label();
            lblPeriod->Text = L"Период:";
            lblPeriod->Location = System::Drawing::Point(12, 16);
            lblPeriod->Size = System::Drawing::Size(64, 20);

            dtFrom = gcnew DateTimePicker();
            dtFrom->Location = System::Drawing::Point(80, 13);
            dtFrom->Size = System::Drawing::Size(130, 24);
            dtFrom->Format = DateTimePickerFormat::Short;
            dtFrom->Value = DateTime::Now.AddDays(-21);

            dtTo = gcnew DateTimePicker();
            dtTo->Location = System::Drawing::Point(218, 13);
            dtTo->Size = System::Drawing::Size(130, 24);
            dtTo->Format = DateTimePickerFormat::Short;

            Label^ lblType = gcnew Label();
            lblType->Text = L"Вид отчёта:";
            lblType->Location = System::Drawing::Point(364, 16);
            lblType->Size = System::Drawing::Size(86, 20);

            cmbType = gcnew ComboBox();
            cmbType->Location = System::Drawing::Point(454, 13);
            cmbType->Size = System::Drawing::Size(214, 24);
            cmbType->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbType->Items->AddRange(gcnew cli::array<Object^>(4) {
                L"Сводный по ремонтам", L"По видам техники",
                L"По мастерам", L"По запасным частям" });
            cmbType->SelectedIndex = 0;

            btnBuild = MakeBtn(L"Сформировать", 682, 12, 130,
                Color::FromArgb(60, 140, 90), true);
            btnBuild->Click += gcnew EventHandler(this, &ReportForm::btnBuild_Click);
            btnPdf = MakeBtn(L"PDF", 820, 12, 60, Color::White, false);
            btnPdf->Click += gcnew EventHandler(this, &ReportForm::btnExport_Click);
            btnExcel = MakeBtn(L"Excel", 820, 12, 60, Color::White, false);
            btnExcel->Location = System::Drawing::Point(820, 12);

            pnlKpi = gcnew Panel();
            pnlKpi->Location = System::Drawing::Point(12, 48);
            pnlKpi->Size = System::Drawing::Size(892, 86);

            chart = gcnew Chart();
            chart->Location = System::Drawing::Point(12, 144);
            chart->Size = System::Drawing::Size(440, 230);
            ChartArea^ area = gcnew ChartArea(L"main");
            area->AxisX->MajorGrid->Enabled = false;
            area->AxisY->MajorGrid->LineColor = Color::FromArgb(224, 228, 233);
            chart->ChartAreas->Add(area);
            Title^ t = gcnew Title(L"Топ-5 видов техники по количеству ремонтов");
            t->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f, FontStyle::Bold);
            chart->Titles->Add(t);
            chart->BackColor = Color::White;

            dgvMasters = gcnew DataGridView();
            dgvMasters->Location = System::Drawing::Point(464, 144);
            dgvMasters->Size = System::Drawing::Size(440, 230);
            dgvMasters->AllowUserToAddRows = false;
            dgvMasters->ReadOnly = true;
            dgvMasters->RowHeadersVisible = false;
            dgvMasters->BackgroundColor = Color::White;
            dgvMasters->BorderStyle = BorderStyle::FixedSingle;
            dgvMasters->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvMasters->ColumnHeadersDefaultCellStyle->BackColor =
                Color::FromArgb(231, 235, 240);
            dgvMasters->EnableHeadersVisualStyles = false;
            dgvMasters->Columns->Add(L"m", L"Мастер");
            dgvMasters->Columns->Add(L"c", L"Заказов");
            dgvMasters->Columns->Add(L"t", L"Ср. срок");
            dgvMasters->Columns->Add(L"b", L"Брак");
            dgvMasters->Columns->Add(L"r", L"Рейтинг");

            grpConclusion = gcnew GroupBox();
            grpConclusion->Text = L"Выводы и рекомендации системы";
            grpConclusion->Location = System::Drawing::Point(12, 382);
            grpConclusion->Size = System::Drawing::Size(892, 88);
            Label^ lc = gcnew Label();
            lc->Location = System::Drawing::Point(14, 22);
            lc->Size = System::Drawing::Size(864, 58);
            lc->Text = L"•  Доля ремонтов, выполненных в срок, выросла на 6,2 п. п. "
                L"по сравнению с предыдущим периодом.\r\n"
                L"•  Основная причина просрочек — отсутствие запчастей на складе "
                L"(9 позиций ниже нормы).\r\n"
                L"•  Рекомендуется увеличить неснижаемый остаток по позициям "
                L"DSP-X515 и BP-650W.";
            grpConclusion->Controls->Add(lc);

            this->ClientSize = System::Drawing::Size(916, 482);
            this->Text = L"РемСервис — отчёты и аналитика (рабочее место руководителя)";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(lblPeriod); this->Controls->Add(dtFrom);
            this->Controls->Add(dtTo); this->Controls->Add(lblType);
            this->Controls->Add(cmbType); this->Controls->Add(btnBuild);
            this->Controls->Add(btnPdf);
            this->Controls->Add(pnlKpi); this->Controls->Add(chart);
            this->Controls->Add(dgvMasters); this->Controls->Add(grpConclusion);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        Button^ MakeBtn(String^ text, int x, int y, int w, Color back, bool filled)
        {
            Button^ b = gcnew Button();
            b->Text = text; b->Location = System::Drawing::Point(x, y);
            b->Size = System::Drawing::Size(w, 27);
            b->FlatStyle = FlatStyle::Flat;
            if (filled) { b->BackColor = back; b->ForeColor = Color::White; }
            return b;
        }

        Panel^ Tile(int x, String^ value, String^ caption, Color back, Color border)
        {
            Panel^ p = gcnew Panel();
            p->Location = System::Drawing::Point(x, 4);
            p->Size = System::Drawing::Size(168, 76);
            p->BackColor = back; p->BorderStyle = BorderStyle::FixedSingle;
            Label^ lv = gcnew Label(); lv->Text = value;
            lv->Font = gcnew System::Drawing::Font(L"Segoe UI", 17, FontStyle::Bold);
            lv->ForeColor = Color::FromArgb(36, 48, 63);
            lv->TextAlign = ContentAlignment::MiddleCenter;
            lv->Dock = DockStyle::Top; lv->Height = 42;
            Label^ lc = gcnew Label(); lc->Text = caption;
            lc->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.25f);
            lc->ForeColor = Color::FromArgb(74, 86, 101);
            lc->TextAlign = ContentAlignment::TopCenter; lc->Dock = DockStyle::Fill;
            p->Controls->Add(lc); p->Controls->Add(lv);
            return p;
        }

        void LoadKpi()
        {
            pnlKpi->Controls->Add(Tile(0, L"246", L"Заказ-нарядов за период",
                Color::FromArgb(218, 232, 252), Color::FromArgb(108, 142, 191)));
            pnlKpi->Controls->Add(Tile(180, L"218", L"Выполнено в срок (88,6 %)",
                Color::FromArgb(213, 232, 212), Color::FromArgb(130, 179, 102)));
            pnlKpi->Controls->Add(Tile(360, L"4,6 дн", L"Средний срок ремонта",
                Color::FromArgb(176, 227, 230), Color::FromArgb(14, 128, 136)));
            pnlKpi->Controls->Add(Tile(540, L"9 720 ₽", L"Средний чек",
                Color::FromArgb(225, 213, 231), Color::FromArgb(150, 115, 166)));
            pnlKpi->Controls->Add(Tile(720, L"2 391 000 ₽", L"Выручка за период",
                Color::FromArgb(255, 230, 204), Color::FromArgb(215, 155, 0)));
        }

        void LoadChart()
        {
            Series^ s = gcnew Series(L"Ремонты");
            s->ChartType = SeriesChartType::Bar;
            s->Color = Color::FromArgb(108, 142, 191);
            s->IsValueShownAsLabel = true;
            s->Points->AddXY(L"Телевизоры", 27);
            s->Points->AddXY(L"Бытовая техника", 38);
            s->Points->AddXY(L"Печатающая техника", 42);
            s->Points->AddXY(L"Смартфоны и планшеты", 61);
            s->Points->AddXY(L"Ноутбуки и ПК", 78);
            chart->Series->Add(s);
        }

        void LoadMasters()
        {
            dgvMasters->Rows->Add(L"Петров С. В.", L"64", L"4,1 дн", L"1", L"4,9");
            dgvMasters->Rows->Add(L"Николаев Р. Д.", L"58", L"4,4 дн", L"2", L"4,8");
            dgvMasters->Rows->Add(L"Гришин П. А.", L"51", L"5,2 дн", L"5", L"4,4");
            dgvMasters->Rows->Add(L"Белова Т. Ю.", L"39", L"4,8 дн", L"1", L"4,7");
            dgvMasters->Rows->Add(L"Сафин Р. И.", L"34", L"5,0 дн", L"3", L"4,5");
        }

        System::Void btnBuild_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(String::Format(
                L"Сформирован отчёт «{0}»\nза период с {1} по {2}.",
                cmbType->Text, dtFrom->Value.ToShortDateString(),
                dtTo->Value.ToShortDateString()),
                L"Отчёт сформирован", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnExport_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Отчёт выгружен в файл report_22_09_2026.pdf.",
                L"Экспорт", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
