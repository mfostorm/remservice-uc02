@echo off
chcp 65001 >nul
title РемСервис — выгрузка проекта на GitHub
echo.
echo ============================================================
echo   Выгрузка проекта «РемСервис» (УП.02, вариант 20) на GitHub
echo ============================================================
echo.
echo   Перед запуском создайте ПУСТОЙ репозиторий на github.com:
echo     New repository  ^>  имя: remservice-uc02  ^>  Public
echo     НЕ ставьте галочки Add README / .gitignore / license
echo.
set /p REPO="Вставьте HTTPS-адрес репозитория и нажмите Enter: "
echo.
set /p UNAME="Ваше имя для коммитов (например Ivanov Ivan): "
set /p UMAIL="Ваш e-mail на GitHub: "
echo.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0init_and_push.ps1" -RepoUrl "%REPO%" -UserName "%UNAME%" -UserEmail "%UMAIL%"
echo.
pause
