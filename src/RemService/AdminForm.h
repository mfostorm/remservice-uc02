#pragma once

namespace RemService {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    /// <summary>
    /// Форма администратора: управление пользователями, ролями и правами доступа,
    /// просмотр журнала действий (аудита).
    /// </summary>
    public ref class AdminForm : public System::Windows::Forms::Form
    {
    public:
        AdminForm(void) { InitializeComponent(); LoadUsers(); LoadAudit(); ShowRights(); }
    protected:
        ~AdminForm() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;
        GroupBox^ grpUsers; GroupBox^ grpRights; GroupBox^ grpAudit;
        DataGridView^ dgvUsers; DataGridView^ dgvAudit;
        CheckedListBox^ clbRights;
        Button^ btnAdd; Button^ btnEdit; Button^ btnResetPwd; Button^ btnBlock;
        Button^ btnSaveRights;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            grpUsers = gcnew GroupBox();
            grpUsers->Text = L"Пользователи системы";
            grpUsers->Location = System::Drawing::Point(12, 12);
            grpUsers->Size = System::Drawing::Size(560, 262);

            dgvUsers = gcnew DataGridView();
            dgvUsers->Location = System::Drawing::Point(12, 24);
            dgvUsers->Size = System::Drawing::Size(536, 190);
            ConfigureGrid(dgvUsers);
            dgvUsers->Columns->Add(L"login", L"Логин");
            dgvUsers->Columns->Add(L"fio", L"ФИО");
            dgvUsers->Columns->Add(L"pos", L"Должность");
            dgvUsers->Columns->Add(L"role", L"Роль");
            dgvUsers->Columns->Add(L"state", L"Статус");
            dgvUsers->SelectionChanged +=
                gcnew EventHandler(this, &AdminForm::dgvUsers_SelectionChanged);

            btnAdd = MakeBtn(L"Добавить", 12, 222, 120, Color::FromArgb(60, 140, 90), true);
            btnAdd->Click += gcnew EventHandler(this, &AdminForm::btnAdd_Click);
            btnEdit = MakeBtn(L"Изменить", 140, 222, 120, Color::FromArgb(46, 109, 164), true);
            btnEdit->Click += gcnew EventHandler(this, &AdminForm::btnEdit_Click);
            btnResetPwd = MakeBtn(L"Сбросить пароль", 268, 222, 140, Color::White, false);
            btnResetPwd->Click += gcnew EventHandler(this, &AdminForm::btnResetPwd_Click);
            btnBlock = MakeBtn(L"Блокировать", 416, 222, 130, Color::White, false);
            btnBlock->ForeColor = Color::FromArgb(184, 84, 80);
            btnBlock->Click += gcnew EventHandler(this, &AdminForm::btnBlock_Click);

            grpUsers->Controls->Add(dgvUsers);
            grpUsers->Controls->Add(btnAdd); grpUsers->Controls->Add(btnEdit);
            grpUsers->Controls->Add(btnResetPwd); grpUsers->Controls->Add(btnBlock);

            grpRights = gcnew GroupBox();
            grpRights->Text = L"Права выбранной роли";
            grpRights->Location = System::Drawing::Point(584, 12);
            grpRights->Size = System::Drawing::Size(320, 262);

            clbRights = gcnew CheckedListBox();
            clbRights->Location = System::Drawing::Point(12, 24);
            clbRights->Size = System::Drawing::Size(296, 186);
            clbRights->BorderStyle = BorderStyle::FixedSingle;
            clbRights->CheckOnClick = true;

            btnSaveRights = MakeBtn(L"Сохранить права", 12, 220, 296,
                Color::FromArgb(60, 140, 90), true);
            btnSaveRights->Click += gcnew EventHandler(this, &AdminForm::btnSaveRights_Click);

            grpRights->Controls->Add(clbRights);
            grpRights->Controls->Add(btnSaveRights);

            grpAudit = gcnew GroupBox();
            grpAudit->Text = L"Журнал действий пользователей (аудит)";
            grpAudit->Location = System::Drawing::Point(12, 282);
            grpAudit->Size = System::Drawing::Size(892, 170);

            dgvAudit = gcnew DataGridView();
            dgvAudit->Location = System::Drawing::Point(12, 24);
            dgvAudit->Size = System::Drawing::Size(868, 132);
            ConfigureGrid(dgvAudit);
            dgvAudit->Columns->Add(L"dt", L"Дата и время");
            dgvAudit->Columns->Add(L"user", L"Пользователь");
            dgvAudit->Columns->Add(L"act", L"Действие");
            dgvAudit->Columns->Add(L"obj", L"Объект");
            dgvAudit->Columns->Add(L"ip", L"IP-адрес");
            grpAudit->Controls->Add(dgvAudit);

            this->ClientSize = System::Drawing::Size(916, 464);
            this->Text = L"РемСервис — администрирование: пользователи, роли и права доступа";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(244, 246, 249);
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.75f);
            this->Controls->Add(grpUsers); this->Controls->Add(grpRights);
            this->Controls->Add(grpAudit);
            this->ResumeLayout(false);
        }

        Button^ MakeBtn(String^ text, int x, int y, int w, Color back, bool filled)
        {
            Button^ b = gcnew Button();
            b->Text = text; b->Location = System::Drawing::Point(x, y);
            b->Size = System::Drawing::Size(w, 28);
            b->FlatStyle = FlatStyle::Flat;
            if (filled) { b->BackColor = back; b->ForeColor = Color::White; }
            return b;
        }

        void ConfigureGrid(DataGridView^ g)
        {
            g->AllowUserToAddRows = false;
            g->ReadOnly = true;
            g->RowHeadersVisible = false;
            g->BackgroundColor = Color::White;
            g->BorderStyle = BorderStyle::FixedSingle;
            g->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            g->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            g->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(231, 235, 240);
            g->ColumnHeadersDefaultCellStyle->Font =
                gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            g->EnableHeadersVisualStyles = false;
            g->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(247, 249, 252);
        }

        void AddUser(String^ login, String^ fio, String^ pos, String^ role, bool active)
        {
            int i = dgvUsers->Rows->Add(login, fio, pos, role,
                active ? L"Активен" : L"Заблокирован");
            dgvUsers->Rows[i]->Cells[4]->Style->ForeColor =
                active ? Color::FromArgb(60, 140, 90) : Color::FromArgb(184, 84, 80);
        }

        void LoadUsers()
        {
            AddUser(L"admin", L"Сидоров К. К.", L"Администратор ИС", L"Администратор", true);
            AddUser(L"priemshik1", L"Иванова А. П.", L"Мастер-приёмщик", L"Приёмщик", true);
            AddUser(L"diagnost1", L"Петров С. В.", L"Инженер-диагност", L"Инженер", true);
            AddUser(L"master1", L"Николаев Р. Д.", L"Мастер по ремонту", L"Мастер", true);
            AddUser(L"sklad1", L"Белова Т. Ю.", L"Кладовщик", L"Кладовщик", true);
            AddUser(L"boss", L"Орлова Н. В.", L"Руководитель СЦ", L"Руководитель", true);
            AddUser(L"master2", L"Гришин П. А.", L"Мастер по ремонту", L"Мастер", false);
            if (dgvUsers->Rows->Count > 1) dgvUsers->Rows[1]->Selected = true;
        }

        void LoadAudit()
        {
            dgvAudit->Rows->Add(L"22.09.2026 10:14", L"priemshik1",
                L"Создание заказ-наряда", L"Orders: РН-0249", L"192.168.1.24");
            dgvAudit->Rows->Add(L"22.09.2026 10:02", L"sklad1",
                L"Изменение остатка", L"Parts: BP-450W", L"192.168.1.31");
            dgvAudit->Rows->Add(L"22.09.2026 09:48", L"admin",
                L"Блокировка пользователя", L"Employees: master2", L"192.168.1.10");
            dgvAudit->Rows->Add(L"22.09.2026 09:35", L"diagnost1",
                L"Сохранение дефектной ведомости", L"Diagnostics: РН-0244", L"192.168.1.27");
        }

        /// <summary>Отображение матрицы прав для выбранной роли.</summary>
        void ShowRights()
        {
            clbRights->Items->Clear();
            String^ role = L"Приёмщик";
            if (dgvUsers->CurrentRow != nullptr && dgvUsers->CurrentRow->Cells[3]->Value != nullptr)
                role = dgvUsers->CurrentRow->Cells[3]->Value->ToString();
            grpRights->Text = String::Format(L"Права роли «{0}»", role);

            cli::array<String^>^ names = gcnew cli::array<String^>(8) {
                L"Приём техники и заказ-наряды", L"Согласование стоимости",
                    L"Выдача техники и приём оплаты", L"Просмотр склада запчастей",
                    L"Изменение склада запчастей", L"Диагностика и ремонт",
                    L"Отчёты руководителя", L"Управление пользователями" };

            cli::array<bool>^ mask;
            if (role == L"Администратор")
                mask = gcnew cli::array<bool>(8) { true, true, true, true, true, true, true, true };
            else if (role == L"Приёмщик")
                mask = gcnew cli::array<bool>(8) { true, true, true, true, false, false, false, false };
            else if (role == L"Инженер" || role == L"Мастер")
                mask = gcnew cli::array<bool>(8) { false, false, false, true, false, true, false, false };
            else if (role == L"Кладовщик")
                mask = gcnew cli::array<bool>(8) { false, false, false, true, true, false, false, false };
            else
                mask = gcnew cli::array<bool>(8) { false, false, false, true, false, false, true, false };

            for (int i = 0; i < names->Length; i++)
                clbRights->Items->Add(names[i], mask[i]);
        }

        System::Void dgvUsers_SelectionChanged(System::Object^ sender, System::EventArgs^ e)
        {
            if (clbRights != nullptr) ShowRights();
        }
        System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e)
        {
            AddUser(L"new_user", L"Новый сотрудник", L"Не указана", L"Приёмщик", true);
            MessageBox::Show(L"Выполнен INSERT INTO Employees.\nЗадайте пароль и роль.",
                L"Новый пользователь", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnEdit_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgvUsers->CurrentRow == nullptr) return;
            MessageBox::Show(String::Format(L"Редактирование учётной записи «{0}».",
                dgvUsers->CurrentRow->Cells[0]->Value), L"Изменение",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnResetPwd_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgvUsers->CurrentRow == nullptr) return;
            MessageBox::Show(String::Format(
                L"Пароль пользователя «{0}» сброшен.\nВременный пароль отправлен на e-mail.",
                dgvUsers->CurrentRow->Cells[0]->Value), L"Сброс пароля",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        System::Void btnBlock_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dgvUsers->CurrentRow == nullptr) return;
            DataGridViewRow^ r = dgvUsers->CurrentRow;
            bool active = (r->Cells[4]->Value->ToString() == L"Активен");
            r->Cells[4]->Value = active ? L"Заблокирован" : L"Активен";
            r->Cells[4]->Style->ForeColor = active
                ? Color::FromArgb(184, 84, 80) : Color::FromArgb(60, 140, 90);
        }
        System::Void btnSaveRights_Click(System::Object^ sender, System::EventArgs^ e)
        {
            MessageBox::Show(L"Права роли сохранены в таблице Roles базы RemServiceDB.",
                L"Права доступа", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}
