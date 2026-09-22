#pragma once

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Форма приёма техники и оформления заказ-наряда.
    /// Соответствует бизнес-процессу A1 «Приём техники и оформление заказ-наряда».
    /// </summary>
    public ref class OrderForm : public System::Windows::Forms::Form
    {
    public:
        OrderForm(void) { InitializeComponent(); }
    protected:
        ~OrderForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        GroupBox^ grpClient; GroupBox^ grpDevice; GroupBox^ grpFault;
        TextBox^ txtFio; TextBox^ txtPhone; TextBox^ txtEmail;
        ComboBox^ cmbRegular; ComboBox^ cmbType; TextBox^ txtBrand;
        TextBox^ txtSerial; DateTimePicker^ dtWarranty;
        TextBox^ txtFault; TextBox^ txtComplect; TextBox^ txtDefects;
        DateTimePicker^ dtIn; DateTimePicker^ dtPlan; ComboBox^ cmbReceiver;
        Button^ btnSave; Button^ btnPrint; Button^ btnToDiag; Button^ btnCancel;
        Label^ lblNumber;

        Label^ L(String^ text, int x, int y, int w)
        {
            Label^ l = gcnew Label();
            l->Text = text; l->Location = System::Drawing::Point(x, y);
            l->Size = System::Drawing::Size(w, 20);
            l->ForeColor = Color::FromArgb(90, 100, 114);
            return l;
        }

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            // ── Клиент ──
            grpClient = gcnew GroupBox();
            grpClient->Text = L"Клиент";
            grpClient->Location = System::Drawing::Point(12, 12);
            grpClient->Size = System::Drawing::Size(440, 140);

            txtFio = gcnew TextBox(); txtFio->Location = System::Drawing::Point(180, 28);
            txtFio->Size = System::Drawing::Size(240, 24);
            txtFio->Text = L"Смирнов Андрей Викторович";
            txtPhone = gcnew TextBox(); txtPhone->Location = System::Drawing::Point(180, 58);
            txtPhone->Size = System::Drawing::Size(240, 24);
            txtPhone->Text = L"+7 (912) 345-67-89";
            txtEmail = gcnew TextBox(); txtEmail->Location = System::Drawing::Point(180, 88);
            txtEmail->Size = System::Drawing::Size(240, 24);
            txtEmail->Text = L"smirnov.av@mail.ru";
            cmbRegular = gcnew ComboBox();
            cmbRegular->Location = System::Drawing::Point(180, 108);
            cmbRegular->Size = System::Drawing::Size(240, 24);
            cmbRegular->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbRegular->Items->AddRange(gcnew cli::array<Object^>(3) {
                L"Новый клиент", L"Постоянный клиент, скидка 5 %",
                L"Корпоративный клиент, скидка 10 %" });
            cmbRegular->SelectedIndex = 1;

            grpClient->Controls->Add(L(L"ФИО / организация:", 14, 31, 160));
            grpClient->Controls->Add(txtFio);
            grpClient->Controls->Add(L(L"Телефон:", 14, 61, 160));
            grpClient->Controls->Add(txtPhone);
            grpClient->Controls->Add(L(L"E-mail:", 14, 91, 160));
            grpClient->Controls->Add(txtEmail);
            grpClient->Controls->Add(L(L"Статус клиента:", 14, 111, 160));
            grpClient->Controls->Add(cmbRegular);

            // ── Техника ──
            grpDevice = gcnew GroupBox();
            grpDevice->Text = L"Техника, принимаемая в ремонт";
            grpDevice->Location = System::Drawing::Point(464, 12);
            grpDevice->Size = System::Drawing::Size(440, 140);

            cmbType = gcnew ComboBox();
            cmbType->Location = System::Drawing::Point(180, 28);
            cmbType->Size = System::Drawing::Size(240, 24);
            cmbType->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbType->Items->AddRange(gcnew cli::array<Object^>(6) {
                L"Ноутбук", L"Персональный компьютер", L"Смартфон / планшет",
                L"Печатающая техника", L"Бытовая техника", L"Телевизор" });
            cmbType->SelectedIndex = 0;
            txtBrand = gcnew TextBox(); txtBrand->Location = System::Drawing::Point(180, 58);
            txtBrand->Size = System::Drawing::Size(240, 24); txtBrand->Text = L"ASUS X515EA";
            txtSerial = gcnew TextBox(); txtSerial->Location = System::Drawing::Point(180, 88);
            txtSerial->Size = System::Drawing::Size(240, 24);
            txtSerial->Text = L"M1N0CV02R441892";
            dtWarranty = gcnew DateTimePicker();
            dtWarranty->Location = System::Drawing::Point(180, 108);
            dtWarranty->Size = System::Drawing::Size(240, 24);
            dtWarranty->Format = DateTimePickerFormat::Short;

            grpDevice->Controls->Add(L(L"Тип техники:", 14, 31, 160));
            grpDevice->Controls->Add(cmbType);
            grpDevice->Controls->Add(L(L"Бренд / модель:", 14, 61, 160));
            grpDevice->Controls->Add(txtBrand);
            grpDevice->Controls->Add(L(L"Серийный номер:", 14, 91, 160));
            grpDevice->Controls->Add(txtSerial);
            grpDevice->Controls->Add(L(L"Гарантия до:", 14, 111, 160));
            grpDevice->Controls->Add(dtWarranty);

            // ── Неисправность ──
            grpFault = gcnew GroupBox();
            grpFault->Text = L"Заявленная неисправность, комплектность и сроки";
            grpFault->Location = System::Drawing::Point(12, 160);
            grpFault->Size = System::Drawing::Size(892, 140);

            txtFault = gcnew TextBox(); txtFault->Location = System::Drawing::Point(220, 28);
            txtFault->Size = System::Drawing::Size(220, 24); txtFault->Text = L"Не включается";
            txtComplect = gcnew TextBox(); txtComplect->Location = System::Drawing::Point(220, 58);
            txtComplect->Size = System::Drawing::Size(220, 24);
            txtComplect->Text = L"Блок питания, сумка";
            txtDefects = gcnew TextBox(); txtDefects->Location = System::Drawing::Point(220, 88);
            txtDefects->Size = System::Drawing::Size(220, 24);
            txtDefects->Text = L"Потёртость крышки, скол угла";

            dtIn = gcnew DateTimePicker(); dtIn->Location = System::Drawing::Point(660, 28);
            dtIn->Size = System::Drawing::Size(220, 24);
            dtIn->Format = DateTimePickerFormat::Short;
            dtPlan = gcnew DateTimePicker(); dtPlan->Location = System::Drawing::Point(660, 58);
            dtPlan->Size = System::Drawing::Size(220, 24);
            dtPlan->Format = DateTimePickerFormat::Short;
            dtPlan->Value = DateTime::Now.AddDays(7);
            cmbReceiver = gcnew ComboBox();
            cmbReceiver->Location = System::Drawing::Point(660, 88);
            cmbReceiver->Size = System::Drawing::Size(220, 24);
            cmbReceiver->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbReceiver->Items->AddRange(gcnew cli::array<Object^>(2) {
                L"Иванова А. П.", L"Соколов В. Г." });
            cmbReceiver->SelectedIndex = 0;

            grpFault->Controls->Add(L(L"Неисправность со слов клиента:", 14, 31, 200));
            grpFault->Controls->Add(txtFault);
            grpFault->Controls->Add(L(L"Комплектность:", 14, 61, 200));
            grpFault->Controls->Add(txtComplect);
            grpFault->Controls->Add(L(L"Внешние дефекты:", 14, 91, 200));
            grpFault->Controls->Add(txtDefects);
            grpFault->Controls->Add(L(L"Дата приёма:", 460, 31, 190));
            grpFault->Controls->Add(dtIn);
            grpFault->Controls->Add(L(L"Плановый срок готовности:", 460, 61, 190));
            grpFault->Controls->Add(dtPlan);
            grpFault->Controls->Add(L(L"Принял (мастер-приёмщик):", 460, 91, 190));
            grpFault->Controls->Add(cmbReceiver);

            // ── Кнопки ──
            btnSave = gcnew Button(); btnSave->Text = L"Сохранить заказ-наряд";
            btnSave->Location = System::Drawing::Point(12, 312);
            btnSave->Size = System::Drawing::Size(190, 32);
            btnSave->BackColor = Color::FromArgb(60, 140, 90);
            btnSave->ForeColor = Color::White; btnSave->FlatStyle = FlatStyle::Flat;
            btnSave->Click += gcnew EventHandler(this, &OrderForm::btnSave_Click);

            btnPrint = gcnew Button(); btnPrint->Text = L"Печать квитанции";
            btnPrint->Location = System::Drawing::Point(210, 312);
            btnPrint->Size = System::Drawing::Size(170, 32);
            btnPrint->FlatStyle = FlatStyle::Flat;
            btnPrint->Click += gcnew EventHandler(this, &OrderForm::btnPrint_Click);

            btnToDiag = gcnew Button(); btnToDiag->Text = L"Отправить на диагностику";
            btnToDiag->Location = System::Drawing::Point(388, 312);
            btnToDiag->Size = System::Drawing::Size(200, 32);
            btnToDiag->FlatStyle = FlatStyle::Flat;
            btnToDiag->Click += gcnew EventHandler(this, &OrderForm::btnToDiag_Click);

            btnCancel = gcnew Button(); btnCancel->Text = L"Отмена";
            btnCancel->Location = System::Drawing::Point(596, 312);
            btnCancel->Size = System::Drawing::Size(120, 32);
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->Click += gcnew EventHandler(this, &OrderForm::btnCancel_Click);

            lblNumber = gcnew Label();
            lblNumber->Text = L"№ РН-0249";
            lblNumber->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            lblNumber->ForeColor = Color::FromArgb(46, 109, 164);
            lblNumber->Location = System::Drawing::Point(780, 318);
            lblNumber->AutoSize = true;

            this->ClientSize = System::Drawing::Size(916, 360);
            this->Text = L"РемСервис — приём техники и оформление заказ-наряда";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(grpClient); this->Controls->Add(grpDevice);
            this->Controls->Add(grpFault); this->Controls->Add(btnSave);
            this->Controls->Add(btnPrint); this->Controls->Add(btnToDiag);
            this->Controls->Add(btnCancel); this->Controls->Add(lblNumber);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        System::Void btnSave_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtFio->Text) ||
                String::IsNullOrWhiteSpace(txtFault->Text))
            {
                MessageBox::Show(L"Заполните ФИО клиента и описание неисправности.",
                    L"Проверка данных", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            MessageBox::Show(String::Format(
                L"Заказ-наряд {0} сохранён.\n\nКлиент: {1}\nТехника: {2} {3}\n"
                L"Неисправность: {4}\nПлановый срок: {5}",
                lblNumber->Text, txtFio->Text, cmbType->Text, txtBrand->Text,
                txtFault->Text, dtPlan->Value.ToShortDateString()),
                L"Заказ-наряд сохранён", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnPrint_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Квитанция о приёме техники отправлена на печать.",
                L"Печать", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnToDiag_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Заказ-наряд передан инженеру-диагносту.\n"
                L"Статус изменён на «Диагностика».",
                L"Передача в работу", MessageBoxButtons::OK, MessageBoxIcon::Information);
            this->Close();
        }
        System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->Close();
        }
    };
}
