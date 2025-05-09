#!/bin/bash

# 设置颜色输出
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}开始构建项目...${NC}"

# 创建并进入构建目录
mkdir -p build
cd build

# 运行 CMake 配置
echo -e "${GREEN}运行 CMake 配置...${NC}"
cmake ..

# 检查 CMake 配置是否成功
if [ $? -ne 0 ]; then
    echo -e "${RED}CMake 配置失败！${NC}"
    exit 1
fi

# 编译项目
echo -e "${GREEN}开始编译...${NC}"
cmake --build .

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo -e "${GREEN}编译成功！${NC}"
    echo -e "${GREEN}开始运行程序...${NC}"
    echo "----------------------------------------"
    ./InputSystem
else
    echo -e "${RED}编译失败！${NC}"
    exit 1
fi 