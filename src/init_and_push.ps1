<#
.SYNOPSIS
    Создаёт локальный Git-репозиторий проекта «РемСервис» (УП.02, вариант 20),
    раскладывает материалы по веткам main, case_task1…case_task4 и отправляет их на GitHub.

.EXAMPLE
    .\init_and_push.ps1 -RepoUrl https://github.com/ivanov/remservice-uc02.git

.NOTES
    Перед запуском создайте ПУСТОЙ репозиторий на github.com
    (без README, без .gitignore, без лицензии) и скопируйте его HTTPS-адрес.
#>

param(
    [Parameter(Mandatory = $true)]
    [string]$RepoUrl,

    [string]$UserName  = "",
    [string]$UserEmail = "",

    [string]$WorkDir   = "$PSScriptRoot\build"
)

$ErrorActionPreference = "Continue"   # git пишет прогресс в stderr
$ProgressPreference    = "SilentlyContinue"

function Step($text) {
    Write-Host ""
    Write-Host ("=== " + $text) -ForegroundColor Cyan
}

# ── 0. Проверки ──────────────────────────────────────────────────────────────
Step "Проверка окружения"
try { git --version | Out-Null }
catch { Write-Host "Git не найден. Установите Git: https://git-scm.com/download/win" -ForegroundColor Red; exit 1 }

$content = Join-Path $PSScriptRoot "_content"
if (-not (Test-Path $content)) {
    Write-Host "Не найден каталог _content рядом со скриптом." -ForegroundColor Red; exit 1
}
Write-Host "Git обнаружен, материалы найдены." -ForegroundColor Green

# ── 1. Подготовка рабочего каталога ──────────────────────────────────────────
Step "Подготовка рабочего каталога"
if (Test-Path $WorkDir) { Remove-Item -Recurse -Force $WorkDir }
New-Item -ItemType Directory -Path $WorkDir | Out-Null
Set-Location $WorkDir

git init | Out-Null
git symbolic-ref HEAD refs/heads/main
if ($UserName)  { git config user.name  $UserName }
if ($UserEmail) { git config user.email $UserEmail }
git config core.quotepath false
git config core.autocrlf false
Write-Host "Репозиторий инициализирован в $WorkDir" -ForegroundColor Green

# ── 2. Ветка main ────────────────────────────────────────────────────────────
Step "Ветка main — README, .gitignore, LICENSE"
Copy-Item "$content\main\*" -Destination $WorkDir -Recurse -Force
git add -A
git commit -m "main: описание проекта, .gitignore и лицензия" | Out-Null
Write-Host "Коммит ветки main создан." -ForegroundColor Green

# ── 3. Ветки кейс-задач ──────────────────────────────────────────────────────
$branches = [ordered]@{
    "case_task1" = "case_task1: модели бизнес-процессов IDEF0, IDEF3, DFD и восемь диаграмм UML"
    "case_task2" = "case_task2: спецификация ПО, ТЗ по ГОСТ 34.602-2020, прототип интерфейса, сетевой график и Гант"
    "case_task3" = "case_task3: графический интерфейс на C++/CLI (Windows Forms) и скрипт базы данных"
    "case_task4" = "case_task4: организация хранения проекта, схема ветвления и скрипты автоматизации"
}

foreach ($b in $branches.Keys) {
    Step "Ветка $b"
    git checkout -q main
    git checkout -q -b $b
    Copy-Item "$content\$b\*" -Destination $WorkDir -Recurse -Force
    git add -A
    git commit -m $branches[$b] | Out-Null
    Write-Host "Коммит ветки $b создан." -ForegroundColor Green
}

git checkout -q main

# ── 4. Отправка на GitHub ────────────────────────────────────────────────────
Step "Отправка на GitHub: $RepoUrl"
if ((git remote) -contains "origin") { git remote remove origin | Out-Null }
git remote add origin $RepoUrl

$failed = @()
foreach ($b in @("main") + @($branches.Keys)) {
    $out = git push -u origin $b 2>&1 | Out-String
    if ($LASTEXITCODE -ne 0) {
        Write-Host ("  ветка " + $b + " — ОШИБКА") -ForegroundColor Red
        Write-Host $out
        $failed += $b
    } else {
        Write-Host ("  ветка " + $b + " отправлена") -ForegroundColor Green
    }
}

if ($failed.Count -gt 0) {
    Write-Host ""
    Write-Host ("Не удалось отправить ветки: " + ($failed -join ", ")) -ForegroundColor Red
    Write-Host "Проверьте адрес репозитория и авторизацию в GitHub, затем выполните:" -ForegroundColor Yellow
    Write-Host ("  cd `"" + $WorkDir + "`"")
    Write-Host "  git push -u origin --all"
    exit 1
}

Step "Готово"
Write-Host "Все пять веток отправлены в репозиторий:" -ForegroundColor Green
Write-Host "  main, case_task1, case_task2, case_task3, case_task4"
Write-Host ""
Write-Host "Откройте репозиторий в браузере и сделайте скриншоты:" -ForegroundColor Yellow
Write-Host "  1) главная страница с README"
Write-Host "  2) страница ветвей:  $($RepoUrl -replace '\.git$','')/branches"
Write-Host "  3) содержимое ветки case_task1 и case_task3"
Write-Host "  4) история коммитов: $($RepoUrl -replace '\.git$','')/commits"
Write-Host "  5) граф ветвления:   $($RepoUrl -replace '\.git$','')/network"
