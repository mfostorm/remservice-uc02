#pragma once
#include "MainForm.h"

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Форма авторизации пользователя ИС «РемСервис».
    /// Реализует вход с проверкой логина, пароля и выбранной роли.
    /// </summary>
    public ref class LoginForm : public System::Windows::Forms::Form
    {
    public:
        LoginForm(void) { InitializeComponent(); }

    protected:
        ~LoginForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        Label^ lblTitle;
        Label^ lblSubtitle;
        Label^ lblLogin;
        Label^ lblPassword;
        Label^ lblRole;
        TextBox^ txtLogin;
        TextBox^ txtPassword;
        ComboBox^ cmbRole;
        Button^ btnLogin;
        Button^ btnExit;
        Panel^ pnlHeader;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->pnlHeader = gcnew Panel();
            this->lblTitle = gcnew Label();
            this->lblSubtitle = gcnew Label();
            this->lblLogin = gcnew Label();
            this->lblPassword = gcnew Label();
            this->lblRole = gcnew Label();
            this->txtLogin = gcnew TextBox();
            this->txtPassword = gcnew TextBox();
            this->cmbRole = gcnew ComboBox();
            this->btnLogin = gcnew Button();
            this->btnExit = gcnew Button();
            this->SuspendLayout();

            // Шапка формы
            this->pnlHeader->BackColor = Color::FromArgb(46, 109, 164);
            this->pnlHeader->Dock = DockStyle::Top;
            this->pnlHeader->Height = 64;

            this->lblTitle->Text = L"РемСервис";
            this->lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
            this->lblTitle->ForeColor = Color::White;
            this->lblTitle->Location = System::Drawing::Point(24, 8);
            this->lblTitle->AutoSize = true;

            this->lblSubtitle->Text = L"Информационная система сервисного центра по ремонту техники";
            this->lblSubtitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f);
            this->lblSubtitle->ForeColor = Color::FromArgb(214, 229, 244);
            this->lblSubtitle->Location = System::Drawing::Point(26, 40);
            this->lblSubtitle->AutoSize = true;
            this->pnlHeader->Controls->Add(this->lblTitle);
            this->pnlHeader->Controls->Add(this->lblSubtitle);

            // Поля ввода
            this->lblLogin->Text = L"Логин:";
            this->lblLogin->Location = System::Drawing::Point(48, 104);
            this->lblLogin->Size = System::Drawing::Size(80, 20);

            this->txtLogin->Location = System::Drawing::Point(140, 101);
            this->txtLogin->Size = System::Drawing::Size(220, 24);
            this->txtLogin->Text = L"priemshik1";

            this->lblPassword->Text = L"Пароль:";
            this->lblPassword->Location = System::Drawing::Point(48, 140);
            this->lblPassword->Size = System::Drawing::Size(80, 20);

            this->txtPassword->Location = System::Drawing::Point(140, 137);
            this->txtPassword->Size = System::Drawing::Size(220, 24);
            this->txtPassword->PasswordChar = '*';
            this->txtPassword->Text = L"12345";

            this->lblRole->Text = L"Роль:";
            this->lblRole->Location = System::Drawing::Point(48, 176);
            this->lblRole->Size = System::Drawing::Size(80, 20);

            this->cmbRole->Location = System::Drawing::Point(140, 173);
            this->cmbRole->Size = System::Drawing::Size(220, 24);
            this->cmbRole->DropDownStyle = ComboBoxStyle::DropDownList;
            this->cmbRole->Items->AddRange(gcnew cli::array<Object^>(6) {
                L"Приёмщик", L"Инженер-диагност", L"Мастер по ремонту",
                L"Кладовщик", L"Руководитель", L"Администратор" });
            this->cmbRole->SelectedIndex = 0;

            // Кнопки
            this->btnLogin->Text = L"Войти";
            this->btnLogin->Location = System::Drawing::Point(140, 220);
            this->btnLogin->Size = System::Drawing::Size(104, 32);
            this->btnLogin->BackColor = Color::FromArgb(60, 140, 90);
            this->btnLogin->ForeColor = Color::White;
            this->btnLogin->FlatStyle = FlatStyle::Flat;
            this->btnLogin->Click += gcnew EventHandler(this, &LoginForm::btnLogin_Click);

            this->btnExit->Text = L"Выход";
            this->btnExit->Location = System::Drawing::Point(256, 220);
            this->btnExit->Size = System::Drawing::Size(104, 32);
            this->btnExit->FlatStyle = FlatStyle::Flat;
            this->btnExit->Click += gcnew EventHandler(this, &LoginForm::btnExit_Click);

            // Форма
            this->ClientSize = System::Drawing::Size(420, 288);
            this->Text = L"РемСервис — вход в систему";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(this->pnlHeader);
            this->Controls->Add(this->lblLogin);
            this->Controls->Add(this->txtLogin);
            this->Controls->Add(this->lblPassword);
            this->Controls->Add(this->txtPassword);
            this->Controls->Add(this->lblRole);
            this->Controls->Add(this->cmbRole);
            this->Controls->Add(this->btnLogin);
            this->Controls->Add(this->btnExit);
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        /// <summary>Проверка учётных данных и переход к главной форме.</summary>
        System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(txtLogin->Text) ||
                String::IsNullOrWhiteSpace(txtPassword->Text))
            {
                MessageBox::Show(L"Заполните логин и пароль.", L"Проверка данных",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Демонстрационная проверка (в рабочей версии — обращение к таблице Employees)
            if ((txtLogin->Text == L"priemshik1" && txtPassword->Text == L"12345") ||
                (txtLogin->Text == L"admin" && txtPassword->Text == L"admin"))
            {
                MainForm^ mainForm = gcnew MainForm(cmbRole->Text, txtLogin->Text);
                this->Hide();
                mainForm->ShowDialog();
                this->Close();
            }
            else
            {
                MessageBox::Show(L"Неверный логин или пароль.\nПроверьте раскладку клавиатуры.",
                    L"Ошибка авторизации", MessageBoxButtons::OK, MessageBoxIcon::Error);
                txtPassword->Clear();
                txtPassword->Focus();
            }
        }

        System::Void btnExit_Click(System::Object^ sender, System::EventArgs^ e)
        {
            Application::Exit();
        }
    };
}
