/* =====================================================================
   RemServiceDB.sql
   База данных информационной системы сервисного центра «РемСервис»
   Учебная практика УП.02, вариант 20
   СУБД: Microsoft SQL Server 2019 и выше
   ===================================================================== */

IF DB_ID('RemServiceDB') IS NOT NULL
BEGIN
    ALTER DATABASE RemServiceDB SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE RemServiceDB;
END
GO
CREATE DATABASE RemServiceDB;
GO
USE RemServiceDB;
GO

/* ------------------------- Справочники ------------------------- */

CREATE TABLE Roles (
    id_role      INT IDENTITY(1,1) PRIMARY KEY,
    role_name    NVARCHAR(50)  NOT NULL UNIQUE,
    permissions  NVARCHAR(255) NULL
);

CREATE TABLE Employees (
    id_employee   INT IDENTITY(1,1) PRIMARY KEY,
    id_role       INT           NOT NULL,
    fio           NVARCHAR(100) NOT NULL,
    position      NVARCHAR(50)  NOT NULL,
    login         NVARCHAR(50)  NOT NULL UNIQUE,
    password_hash NVARCHAR(128) NOT NULL,
    rate          DECIMAL(8,2)  NOT NULL DEFAULT 0,
    is_active     BIT           NOT NULL DEFAULT 1,
    CONSTRAINT FK_Employees_Roles FOREIGN KEY (id_role) REFERENCES Roles(id_role)
);

CREATE TABLE Clients (
    id_client INT IDENTITY(1,1) PRIMARY KEY,
    fio       NVARCHAR(100) NOT NULL,
    phone     NVARCHAR(20)  NOT NULL,
    email     NVARCHAR(60)  NULL,
    address   NVARCHAR(120) NULL,
    discount  DECIMAL(4,2)  NOT NULL DEFAULT 0
        CONSTRAINT CK_Clients_Discount CHECK (discount BETWEEN 0 AND 50)
);

CREATE TABLE Devices (
    id_device     INT IDENTITY(1,1) PRIMARY KEY,
    id_client     INT          NOT NULL,
    dev_type      NVARCHAR(40) NOT NULL,
    brand         NVARCHAR(40) NOT NULL,
    model         NVARCHAR(40) NOT NULL,
    serial_number NVARCHAR(40) NULL,
    purchase_date DATE         NULL,
    CONSTRAINT FK_Devices_Clients FOREIGN KEY (id_client)
        REFERENCES Clients(id_client) ON DELETE CASCADE
);

CREATE TABLE Services (
    id_service   INT IDENTITY(1,1) PRIMARY KEY,
    service_name NVARCHAR(80)  NOT NULL,
    norm_hours   DECIMAL(5,2)  NOT NULL,
    price        DECIMAL(10,2) NOT NULL
        CONSTRAINT CK_Services_Price CHECK (price >= 0)
);

CREATE TABLE Parts (
    id_part   INT IDENTITY(1,1) PRIMARY KEY,
    article   NVARCHAR(30)  NOT NULL UNIQUE,
    part_name NVARCHAR(80)  NOT NULL,
    category  NVARCHAR(40)  NOT NULL,
    price     DECIMAL(10,2) NOT NULL,
    stock     INT           NOT NULL DEFAULT 0
        CONSTRAINT CK_Parts_Stock CHECK (stock >= 0),
    min_stock INT           NOT NULL DEFAULT 0,
    supplier  NVARCHAR(60)  NULL
);

/* ------------------------- Операционные таблицы ------------------------- */

CREATE TABLE Orders (
    id_order       INT IDENTITY(1,1) PRIMARY KEY,
    order_number   NVARCHAR(20) NOT NULL UNIQUE,
    id_device      INT          NOT NULL,
    id_client      INT          NOT NULL,
    id_employee    INT          NOT NULL,
    date_in        DATE         NOT NULL DEFAULT GETDATE(),
    date_plan      DATE         NULL,
    date_out       DATE         NULL,
    status         NVARCHAR(30) NOT NULL DEFAULT N'Принят',
    fault_declared NVARCHAR(255) NULL,
    total_sum      DECIMAL(10,2) NOT NULL DEFAULT 0,
    warranty_until DATE          NULL,
    CONSTRAINT FK_Orders_Devices   FOREIGN KEY (id_device)   REFERENCES Devices(id_device),
    CONSTRAINT FK_Orders_Clients   FOREIGN KEY (id_client)   REFERENCES Clients(id_client),
    CONSTRAINT FK_Orders_Employees FOREIGN KEY (id_employee) REFERENCES Employees(id_employee),
    CONSTRAINT CK_Orders_Status CHECK (status IN
        (N'Принят', N'Диагностика', N'Ожидает согласования', N'В ремонте',
         N'Ожидает запчасть', N'Контроль качества', N'На доработке',
         N'Готов к выдаче', N'Выдан', N'Отказ'))
);

CREATE TABLE Diagnostics (
    id_diag     INT IDENTITY(1,1) PRIMARY KEY,
    id_order    INT           NOT NULL,
    id_employee INT           NOT NULL,
    fault_desc  NVARCHAR(255) NOT NULL,
    conclusion  NVARCHAR(255) NOT NULL,
    diag_date   DATE          NOT NULL DEFAULT GETDATE(),
    CONSTRAINT FK_Diag_Orders    FOREIGN KEY (id_order)    REFERENCES Orders(id_order)
        ON DELETE CASCADE,
    CONSTRAINT FK_Diag_Employees FOREIGN KEY (id_employee) REFERENCES Employees(id_employee)
);

CREATE TABLE OrderServices (
    id_order_service INT IDENTITY(1,1) PRIMARY KEY,
    id_order   INT NOT NULL,
    id_service INT NOT NULL,
    qty        INT NOT NULL DEFAULT 1,
    line_sum   DECIMAL(10,2) NOT NULL,
    CONSTRAINT FK_OS_Orders   FOREIGN KEY (id_order)   REFERENCES Orders(id_order)
        ON DELETE CASCADE,
    CONSTRAINT FK_OS_Services FOREIGN KEY (id_service) REFERENCES Services(id_service)
);

CREATE TABLE PartUsage (
    id_usage INT IDENTITY(1,1) PRIMARY KEY,
    id_order INT NOT NULL,
    id_part  INT NOT NULL,
    qty      INT NOT NULL DEFAULT 1,
    line_sum DECIMAL(10,2) NOT NULL,
    CONSTRAINT FK_PU_Orders FOREIGN KEY (id_order) REFERENCES Orders(id_order)
        ON DELETE CASCADE,
    CONSTRAINT FK_PU_Parts  FOREIGN KEY (id_part)  REFERENCES Parts(id_part)
);

CREATE TABLE Payments (
    id_payment INT IDENTITY(1,1) PRIMARY KEY,
    id_order   INT NOT NULL,
    pay_sum    DECIMAL(10,2) NOT NULL,
    method     NVARCHAR(20)  NOT NULL DEFAULT N'Наличные',
    pay_date   DATETIME      NOT NULL DEFAULT GETDATE(),
    CONSTRAINT FK_Pay_Orders FOREIGN KEY (id_order) REFERENCES Orders(id_order)
);

/* ------------------------- Индексы ------------------------- */
CREATE INDEX IX_Orders_Status   ON Orders(status);
CREATE INDEX IX_Orders_DateIn   ON Orders(date_in);
CREATE INDEX IX_Parts_Category  ON Parts(category);
CREATE INDEX IX_Devices_Client  ON Devices(id_client);
GO

/* ------------------------- Наполнение ------------------------- */

INSERT INTO Roles (role_name, permissions) VALUES
 (N'Администратор', N'all'),
 (N'Приёмщик',      N'orders.create;orders.issue;parts.read'),
 (N'Инженер',       N'diagnostics.create;orders.read;parts.read'),
 (N'Мастер',        N'repair.execute;orders.read;parts.read'),
 (N'Кладовщик',     N'parts.read;parts.write;supplies.write'),
 (N'Руководитель',  N'reports.read;orders.read');

INSERT INTO Employees (id_role, fio, position, login, password_hash, rate) VALUES
 (1, N'Сидоров К. К.',  N'Администратор ИС',  N'admin',      N'9A8B7C6D5E', 60000),
 (2, N'Иванова А. П.',  N'Мастер-приёмщик',   N'priemshik1', N'1A2B3C4D5E', 45000),
 (3, N'Петров С. В.',   N'Инженер-диагност',  N'diagnost1',  N'2B3C4D5E6F', 55000),
 (4, N'Николаев Р. Д.', N'Мастер по ремонту', N'master1',    N'3C4D5E6F7A', 52000),
 (5, N'Белова Т. Ю.',   N'Кладовщик',         N'sklad1',     N'4D5E6F7A8B', 40000),
 (6, N'Орлова Н. В.',   N'Руководитель СЦ',   N'boss',       N'5E6F7A8B9C', 90000);

INSERT INTO Clients (fio, phone, email, address, discount) VALUES
 (N'Смирнов Андрей Викторович', N'+7 (912) 345-67-89', N'smirnov.av@mail.ru',
  N'г. Москва, ул. Ленина, 15-42', 5),
 (N'Кузнецова Елена Игоревна',  N'+7 (903) 112-45-78', N'kuznecova@yandex.ru',
  N'г. Москва, пр. Мира, 8-11', 0),
 (N'ООО «Стройсервис»',         N'+7 (495) 221-33-10', N'office@stroyservice.ru',
  N'г. Москва, Варшавское ш., 42', 10),
 (N'Петров Игорь Сергеевич',    N'+7 (916) 778-90-21', N'petrov.is@gmail.com',
  N'г. Москва, ул. Садовая, 3-7', 0);

INSERT INTO Devices (id_client, dev_type, brand, model, serial_number, purchase_date) VALUES
 (1, N'Ноутбук',            N'ASUS',     N'X515EA',     N'M1N0CV02R441892', '2024-03-14'),
 (2, N'Смартфон',           N'Xiaomi',   N'Redmi 12',   N'XR12-884512',     '2025-01-20'),
 (3, N'Печатающая техника', N'HP',       N'LaserJet M428', N'HP428-99120',  '2023-07-05'),
 (4, N'Компьютер',          N'Сборка',   N'Custom ATX', N'PC-2024-0071',    '2024-11-02');

INSERT INTO Services (service_name, norm_hours, price) VALUES
 (N'Диагностика ПК / ноутбука',       0.5,  800),
 (N'Замена блока питания',            1.0, 1200),
 (N'Чистка системы охлаждения',       1.5, 1500),
 (N'Замена термопасты',               0.5,  600),
 (N'Тестирование под нагрузкой',      0.5,  500),
 (N'Замена дисплейного модуля',       2.0, 2500),
 (N'Замена аккумулятора',             1.0, 1100),
 (N'Замена узла термозакрепления',    2.5, 3000),
 (N'Ремонт системы подачи бумаги',    1.5, 1800),
 (N'Замена клавиатуры ноутбука',      1.0, 1300);

INSERT INTO Parts (article, part_name, category, price, stock, min_stock, supplier) VALUES
 (N'BP-450W',   N'Блок питания ATX 450 Вт',    N'Комплектующие',       2900, 12,  5, N'ООО «Компонент»'),
 (N'BP-650W',   N'Блок питания ATX 650 Вт',    N'Комплектующие',       4100,  3,  5, N'ООО «Компонент»'),
 (N'TP-GD900',  N'Термопаста GD900, 3 г',      N'Расходные материалы',  350, 48, 20, N'ИП Сорокин'),
 (N'DSP-X515',  N'Матрица 15,6" FHD ASUS',     N'Дисплеи',             8700,  0,  2, N'ООО «ТехПартс»'),
 (N'KB-LN-IP3', N'Клавиатура Lenovo IdeaPad 3',N'Клавиатуры',          1950,  6,  3, N'ООО «ТехПартс»'),
 (N'BAT-DV8',   N'Аккумулятор Dyson V8',       N'Аккумуляторы',        5400,  2,  2, N'ООО «БытСервис»'),
 (N'FUS-HP26',  N'Узел термозакрепления HP',   N'Печатающая техника',  6300,  4,  2, N'ООО «ОфисТех»'),
 (N'CAP-DL15',  N'Прокладка бойлера DeLonghi', N'Бытовая техника',      480, 15,  8, N'ООО «БытСервис»');

INSERT INTO Orders (order_number, id_device, id_client, id_employee, date_in, date_plan,
                    status, fault_declared, total_sum) VALUES
 (N'РН-0241', 1, 1, 2, '2026-09-12', '2026-09-19', N'Готов к выдаче', N'Не включается', 7400),
 (N'РН-0242', 2, 2, 2, '2026-09-14', '2026-09-21', N'В ремонте',       N'Разбит дисплей', 11900),
 (N'РН-0243', 3, 3, 2, '2026-09-15', '2026-09-22', N'Ожидает запчасть', N'Не тянет бумагу', 3250),
 (N'РН-0244', 4, 4, 2, '2026-09-16', '2026-09-23', N'Диагностика',      N'Перегрев, шум', 0);

INSERT INTO Diagnostics (id_order, id_employee, fault_desc, conclusion) VALUES
 (1, 3, N'Выход из строя блока питания',   N'Ремонтопригодна'),
 (2, 3, N'Повреждение дисплейного модуля', N'Ремонтопригодна'),
 (4, 3, N'Загрязнение системы охлаждения', N'Ремонтопригодна');

INSERT INTO OrderServices (id_order, id_service, qty, line_sum) VALUES
 (1, 1, 1, 800), (1, 2, 1, 1200), (1, 3, 1, 1500), (1, 4, 1, 600), (1, 5, 1, 500);

INSERT INTO PartUsage (id_order, id_part, qty, line_sum) VALUES
 (1, 1, 1, 2900), (1, 3, 1, 350);

INSERT INTO Payments (id_order, pay_sum, method) VALUES
 (1, 7400, N'Банковская карта');
GO

/* ------------------------- Представления ------------------------- */

CREATE VIEW vw_OrdersRegistry AS
SELECT o.order_number                                   AS [Номер],
       c.fio                                            AS [Клиент],
       d.dev_type + N' ' + d.brand + N' ' + d.model     AS [Техника],
       o.fault_declared                                 AS [Неисправность],
       o.date_in                                        AS [Принят],
       o.date_plan                                      AS [Срок],
       o.status                                         AS [Статус],
       o.total_sum                                      AS [Сумма],
       e.fio                                            AS [Принял]
FROM Orders o
     JOIN Clients   c ON c.id_client   = o.id_client
     JOIN Devices   d ON d.id_device   = o.id_device
     JOIN Employees e ON e.id_employee = o.id_employee;
GO

CREATE VIEW vw_PartsShortage AS
SELECT article AS [Артикул], part_name AS [Наименование], category AS [Категория],
       stock AS [Остаток], min_stock AS [Норма], supplier AS [Поставщик]
FROM Parts
WHERE stock < min_stock;
GO

/* ------------------------- Хранимые процедуры ------------------------- */

CREATE PROCEDURE sp_RecalcOrderTotal @id_order INT
AS
BEGIN
    SET NOCOUNT ON;
    DECLARE @works DECIMAL(10,2) = ISNULL(
        (SELECT SUM(line_sum) FROM OrderServices WHERE id_order = @id_order), 0);
    DECLARE @parts DECIMAL(10,2) = ISNULL(
        (SELECT SUM(line_sum) FROM PartUsage WHERE id_order = @id_order), 0);
    DECLARE @disc DECIMAL(4,2) = ISNULL(
        (SELECT c.discount FROM Orders o JOIN Clients c ON c.id_client = o.id_client
         WHERE o.id_order = @id_order), 0);
    UPDATE Orders
       SET total_sum = (@works + @parts) * (1 - @disc / 100.0)
     WHERE id_order = @id_order;
END
GO

CREATE PROCEDURE sp_ChangeOrderStatus @id_order INT, @new_status NVARCHAR(30)
AS
BEGIN
    SET NOCOUNT ON;
    UPDATE Orders SET status = @new_status WHERE id_order = @id_order;
    IF @new_status = N'Выдан'
        UPDATE Orders
           SET date_out = GETDATE(), warranty_until = DATEADD(MONTH, 6, GETDATE())
         WHERE id_order = @id_order;
END
GO

/* ------------------------- Триггер списания запчастей ------------------------- */

CREATE TRIGGER trg_PartUsage_Decrease
ON PartUsage AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;
    UPDATE p
       SET p.stock = p.stock - i.qty
      FROM Parts p JOIN inserted i ON i.id_part = p.id_part;
END
GO

PRINT N'База данных RemServiceDB успешно создана и наполнена демонстрационными данными.';
GO
