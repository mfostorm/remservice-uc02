#pragma once

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Форма диагностики и калькуляции ремонта.
    /// Соответствует процессам A2 «Диагностика неисправности» и
    /// A3.1 «Согласование стоимости и сроков с клиентом».
    /// </summary>
    public ref class DiagnosticForm : public System::Windows::Forms::Form
    {
    public:
        DiagnosticForm(void) { InitializeComponent(); LoadData(); Recalculate(); }
    protected:
        ~DiagnosticForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        GroupBox^ grpDiag; GroupBox^ grpWork; GroupBox^ grpParts; GroupBox^ grpCalc;
        ComboBox^ cmbEngineer; ComboBox^ cmbConclusion;
        TextBox^ txtFault; DateTimePicker^ dtDiag;
        DataGridView^ dgvWork; DataGridView^ dgvParts;
        Label^ lblWorkSum; Label^ lblPartsSum; Label^ lblDiscount; Label^ lblTotal; Label^ lblTerm;
        Button^ btnAddWork; Button^ btnDelWork; Button^ btnPickPart;
        Button^ btnApprove; Button^ btnReject; Button^ btnNotify;

        Label^ L(String^ text, int x, int y, int w, bool bold)
        {
            Label^ l = gcnew Label();
            l->Text = text; l->Location = System::Drawing::Point(x, y);
            l->Size = System::Drawing::Size(w, 20);
            if (bold) l->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f, FontStyle::Bold);
            else l->ForeColor = Color::FromArgb(90, 100, 114);
            return l;
        }

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            grpDiag = gcnew GroupBox();
            grpDiag->Text = L"Результаты технической диагностики";
            grpDiag->Location = System::Drawing::Point(12, 12);
            grpDiag->Size = System::Drawing::Size(892, 90);

            cmbEngineer = gcnew ComboBox();
            cmbEngineer->Location = System::Drawing::Point(180, 28);
            cmbEngineer->Size = System::Drawing::Size(230, 24);
            cmbEngineer->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbEngineer->Items->AddRange(gcnew cli::array<Object^>(3) {
                L"Петров С. В.", L"Николаев Р. Д.", L"Сафин Р. И." });
            cmbEngineer->SelectedIndex = 0;

            dtDiag = gcnew DateTimePicker();
            dtDiag->Location = System::Drawing::Point(180, 56);
            dtDiag->Size = System::Drawing::Size(230, 24);
            dtDiag->Format = DateTimePickerFormat::Short;

            txtFault = gcnew TextBox();
            txtFault->Location = System::Drawing::Point(650, 28);
            txtFault->Size = System::Drawing::Size(230, 24);
            txtFault->Text = L"Выход из строя блока питания";

            cmbConclusion = gcnew ComboBox();
            cmbConclusion->Location = System::Drawing::Point(650, 56);
            cmbConclusion->Size = System::Drawing::Size(230, 24);
            cmbConclusion->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbConclusion->Items->AddRange(gcnew cli::array<Object^>(2) {
                L"Ремонтопригодна", L"Не подлежит ремонту" });
            cmbConclusion->SelectedIndex = 0;

            grpDiag->Controls->Add(L(L"Инженер-диагност:", 14, 31, 160, false));
            grpDiag->Controls->Add(cmbEngineer);
            grpDiag->Controls->Add(L(L"Дата диагностики:", 14, 59, 160, false));
            grpDiag->Controls->Add(dtDiag);
            grpDiag->Controls->Add(L(L"Выявленная неисправность:", 440, 31, 200, false));
            grpDiag->Controls->Add(txtFault);
            grpDiag->Controls->Add(L(L"Заключение:", 440, 59, 200, false));
            grpDiag->Controls->Add(cmbConclusion);

            // ── Работы ──
            grpWork = gcnew GroupBox();
            grpWork->Text = L"Работы по заказ-наряду";
            grpWork->Location = System::Drawing::Point(12, 110);
            grpWork->Size = System::Drawing::Size(540, 220);
            dgvWork = gcnew DataGridView();
            dgvWork->Location = System::Drawing::Point(12, 24);
            dgvWork->Size = System::Drawing::Size(516, 150);
            ConfigureGrid(dgvWork);
            dgvWork->Columns->Add(L"name", L"Наименование работы");
            dgvWork->Columns->Add(L"nh", L"Н/ч");
            dgvWork->Columns->Add(L"price", L"Цена, ₽");
            dgvWork->Columns->Add(L"qty", L"Кол.");
            dgvWork->Columns->Add(L"sum", L"Сумма, ₽");

            btnAddWork = gcnew Button(); btnAddWork->Text = L"+ Добавить работу";
            btnAddWork->Location = System::Drawing::Point(12, 182);
            btnAddWork->Size = System::Drawing::Size(150, 28);
            btnAddWork->BackColor = Color::FromArgb(60, 140, 90);
            btnAddWork->ForeColor = Color::White; btnAddWork->FlatStyle = FlatStyle::Flat;
            btnAddWork->Click += gcnew EventHandler(this, &DiagnosticForm::btnAddWork_Click);

            btnDelWork = gcnew Button(); btnDelWork->Text = L"Удалить";
            btnDelWork->Location = System::Drawing::Point(170, 182);
            btnDelWork->Size = System::Drawing::Size(110, 28);
            btnDelWork->FlatStyle = FlatStyle::Flat;
            btnDelWork->Click += gcnew EventHandler(this, &DiagnosticForm::btnDelWork_Click);

            grpWork->Controls->Add(dgvWork);
            grpWork->Controls->Add(btnAddWork);
            grpWork->Controls->Add(btnDelWork);

            // ── Запчасти ──
            grpParts = gcnew GroupBox();
            grpParts->Text = L"Запасные части";
            grpParts->Location = System::Drawing::Point(564, 110);
            grpParts->Size = System::Drawing::Size(340, 220);
            dgvParts = gcnew DataGridView();
            dgvParts->Location = System::Drawing::Point(12, 24);
            dgvParts->Size = System::Drawing::Size(316, 150);
            ConfigureGrid(dgvParts);
            dgvParts->Columns->Add(L"art", L"Артикул");
            dgvParts->Columns->Add(L"name", L"Наименование");
            dgvParts->Columns->Add(L"qty", L"Кол.");
            dgvParts->Columns->Add(L"sum", L"Сумма");

            btnPickPart = gcnew Button(); btnPickPart->Text = L"Подобрать со склада";
            btnPickPart->Location = System::Drawing::Point(12, 182);
            btnPickPart->Size = System::Drawing::Size(180, 28);
            btnPickPart->FlatStyle = FlatStyle::Flat;
            btnPickPart->Click += gcnew EventHandler(this, &DiagnosticForm::btnPickPart_Click);
            grpParts->Controls->Add(dgvParts);
            grpParts->Controls->Add(btnPickPart);

            // ── Калькуляция ──
            grpCalc = gcnew GroupBox();
            grpCalc->Text = L"Калькуляция и согласование с клиентом";
            grpCalc->Location = System::Drawing::Point(12, 338);
            grpCalc->Size = System::Drawing::Size(892, 100);

            lblWorkSum = L(L"Работы: 0 ₽", 16, 26, 200, false);
            lblPartsSum = L(L"Запчасти: 0 ₽", 16, 48, 200, false);
            lblDiscount = L(L"Скидка клиента 5 %: 0 ₽", 16, 70, 220, false);
            lblDiscount->ForeColor = Color::FromArgb(184, 84, 80);

            lblTotal = gcnew Label();
            lblTotal->Font = gcnew System::Drawing::Font(L"Segoe UI", 15, FontStyle::Bold);
            lblTotal->ForeColor = Color::FromArgb(36, 48, 63);
            lblTotal->Location = System::Drawing::Point(280, 38);
            lblTotal->AutoSize = true;

            lblTerm = L(L"Срок ремонта: 5 рабочих дней", 284, 70, 300, false);

            btnApprove = gcnew Button(); btnApprove->Text = L"Согласовать";
            btnApprove->Location = System::Drawing::Point(600, 26);
            btnApprove->Size = System::Drawing::Size(130, 30);
            btnApprove->BackColor = Color::FromArgb(60, 140, 90);
            btnApprove->ForeColor = Color::White; btnApprove->FlatStyle = FlatStyle::Flat;
            btnApprove->Click += gcnew EventHandler(this, &DiagnosticForm::btnApprove_Click);

            btnReject = gcnew Button(); btnReject->Text = L"Отказ клиента";
            btnReject->Location = System::Drawing::Point(740, 26);
            btnReject->Size = System::Drawing::Size(136, 30);
            btnReject->FlatStyle = FlatStyle::Flat;
            btnReject->ForeColor = Color::FromArgb(184, 84, 80);
            btnReject->Click += gcnew EventHandler(this, &DiagnosticForm::btnReject_Click);

            btnNotify = gcnew Button(); btnNotify->Text = L"Отправить SMS / e-mail клиенту";
            btnNotify->Location = System::Drawing::Point(600, 62);
            btnNotify->Size = System::Drawing::Size(276, 28);
            btnNotify->FlatStyle = FlatStyle::Flat;
            btnNotify->Click += gcnew EventHandler(this, &DiagnosticForm::btnNotify_Click);

            grpCalc->Controls->Add(lblWorkSum); grpCalc->Controls->Add(lblPartsSum);
            grpCalc->Controls->Add(lblDiscount); grpCalc->Controls->Add(lblTotal);
            grpCalc->Controls->Add(lblTerm); grpCalc->Controls->Add(btnApprove);
            grpCalc->Controls->Add(btnReject); grpCalc->Controls->Add(btnNotify);

            this->ClientSize = System::Drawing::Size(916, 450);
            this->Text = L"РемСервис — диагностика и калькуляция ремонта  |  заказ-наряд № РН-0244";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(grpDiag); this->Controls->Add(grpWork);
            this->Controls->Add(grpParts); this->Controls->Add(grpCalc);
            this->ResumeLayout(false);
        }

        void ConfigureGrid(DataGridView^ g)
        {
            g->AllowUserToAddRows = false;
            g->RowHeadersVisible = false;
            g->BackgroundColor = Color::White;
            g->BorderStyle = BorderStyle::FixedSingle;
            g->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            g->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            g->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(231, 235, 240);
            g->EnableHeadersVisualStyles = false;
            g->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(247, 249, 252);
        }

        void LoadData()
        {
            dgvWork->Rows->Add(L"Диагностика ПК / ноутбука", L"0,5", L"800", L"1", L"800");
            dgvWork->Rows->Add(L"Замена блока питания", L"1,0", L"1200", L"1", L"1200");
            dgvWork->Rows->Add(L"Чистка системы охлаждения", L"1,5", L"1500", L"1", L"1500");
            dgvWork->Rows->Add(L"Замена термопасты", L"0,5", L"600", L"1", L"600");
            dgvWork->Rows->Add(L"Тестирование под нагрузкой", L"0,5", L"500", L"1", L"500");
            dgvParts->Rows->Add(L"BP-450W", L"Блок питания ATX 450 Вт", L"1", L"2900");
            dgvParts->Rows->Add(L"TP-GD900", L"Термопаста GD900, 3 г", L"1", L"350");
        }

        /// <summary>Пересчёт итоговой стоимости ремонта с учётом скидки клиента.</summary>
        void Recalculate()
        {
            double work = 0, parts = 0;
            for each (DataGridViewRow ^ r in dgvWork->Rows)
                if (r->Cells[4]->Value != nullptr)
                    work += Convert::ToDouble(r->Cells[4]->Value);
            for each (DataGridViewRow ^ r in dgvParts->Rows)
                if (r->Cells[3]->Value != nullptr)
                    parts += Convert::ToDouble(r->Cells[3]->Value);
            double discount = (work + parts) * 0.05;
            double total = work + parts - discount;

            lblWorkSum->Text = String::Format(L"Работы: {0:N0} ₽", work);
            lblPartsSum->Text = String::Format(L"Запчасти: {0:N0} ₽", parts);
            lblDiscount->Text = String::Format(L"Скидка клиента 5 %: −{0:N0} ₽", discount);
            lblTotal->Text = String::Format(L"ИТОГО К ОПЛАТЕ: {0:N0} ₽", total);
        }

        System::Void btnAddWork_Click(System::Object^ sender, System::EventArgs^ e)
        {
            dgvWork->Rows->Add(L"Новая работа", L"0,5", L"500", L"1", L"500");
            Recalculate();
        }
        System::Void btnDelWork_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgvWork->CurrentRow != nullptr)
                dgvWork->Rows->Remove(dgvWork->CurrentRow);
            Recalculate();
        }
        System::Void btnPickPart_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Открывается справочник склада запасных частей.",
                L"Склад", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnApprove_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(String::Format(
                L"Стоимость согласована с клиентом.\n{0}\nСтатус заказ-наряда: «В ремонте».",
                lblTotal->Text), L"Согласование", MessageBoxButtons::OK,
                MessageBoxIcon::Information);
        }
        System::Void btnReject_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (MessageBox::Show(L"Клиент отказался от ремонта. Оформить возврат техники?",
                L"Отказ клиента", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
                == System::Windows::Forms::DialogResult::Yes)
            {
                MessageBox::Show(L"Оформлен мотивированный отказ, заказ-наряд закрыт.",
                    L"Отказ оформлен", MessageBoxButtons::OK, MessageBoxIcon::Information);
                this->Close();
            }
        }
        System::Void btnNotify_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Уведомление о стоимости и сроках отправлено клиенту.",
                L"Уведомление", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
