@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo [1/3] 拉取 Git 代码...
git pull --rebase
if errorlevel 1 goto error

echo [2/3] 拉取 Git LFS 资产...
git lfs pull
if errorlevel 1 goto error

echo [3/3] 完成。现在可以打开 AIGame.uproject。
pause
exit /b 0

:error
echo.
echo 拉取失败：请确认已安装 Git LFS，并且当前分支没有未解决冲突。
pause
exit /b 1
