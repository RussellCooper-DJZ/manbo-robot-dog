# 毕业设计论文说明

本目录包含三篇不同角度的毕业设计论文,均基于曼波机器狗项目。

## 📚 论文列表

### 1. 基于STM32的智能四足机器狗设计与实现 (综合版)
- **文件**: `thesis.pdf` (加密版: `thesis_encrypted.zip`)
- **字数**: 约27,000字
- **角度**: 综合性论文,涵盖硬件、软件、算法等各方面
- **特点**: 结构完整,内容全面,适合作为主论文提交
- **章节**:
  1. 引言
  2. 系统总体设计
  3. 硬件系统设计
  4. 软件系统设计
  5. 关键技术实现
  6. 系统测试与分析
  7. 总结与展望

### 2. 基于STM32的四足机器狗运动控制算法研究与实现
- **文件**: `thesis_algorithm_control.md` / `thesis_algorithm_control.tex`
- **字数**: 约25,000字
- **角度**: 侧重算法与控制系统
- **特点**: 深入研究运动控制算法、步态规划、传感器融合等技术
- **章节**:
  1. 绪论
  2. 相关理论基础
  3. 系统总体设计
  4. 运动控制算法设计
  5. 多传感器融合避障算法
  6. 系统实现与测试
  7. 总结与展望

### 3. 基于STM32的四足机器狗嵌入式系统设计与开发
- **文件**: `thesis_embedded_development.md` / `thesis_embedded_development.tex`
- **字数**: 约22,000字
- **角度**: 侧重嵌入式系统开发
- **特点**: 详细介绍STM32外设配置、驱动开发、系统调试等技术
- **章节**:
  1. 绪论
  2. STM32硬件系统设计
  3. STM32软件架构设计
  4. 外设驱动程序开发
  5. 应用程序开发
  6. 系统调试与优化
  7. 总结与展望

## 📖 文件格式说明

### Markdown格式 (.md)
- 纯文本格式,易于编辑和版本控制
- 可以使用任何文本编辑器打开
- 可以通过工具转换为其他格式

### LaTeX格式 (.tex)
- 学术论文标准格式
- 需要LaTeX编译器(如XeLaTeX、pdfLaTeX)编译
- 生成高质量的PDF文档

### PDF格式 (.pdf)
- 最终发布格式
- 可以直接查看和打印
- 第一篇论文已加密保护

## 🔧 LaTeX编译方法

### 方法1: 使用XeLaTeX (推荐,支持中文)

```bash
xelatex thesis_algorithm_control.tex
xelatex thesis_algorithm_control.tex  # 编译两次生成目录
```

### 方法2: 使用pdfLaTeX

```bash
pdflatex thesis_algorithm_control.tex
pdflatex thesis_algorithm_control.tex  # 编译两次生成目录
```

### 方法3: 使用在线LaTeX编辑器

推荐使用 [Overleaf](https://www.overleaf.com/):
1. 注册Overleaf账号
2. 创建新项目
3. 上传.tex文件
4. 点击"Recompile"按钮编译

### 方法4: 使用本地LaTeX发行版

**Windows**: 安装 [TeX Live](https://www.tug.org/texlive/) 或 [MiKTeX](https://miktex.org/)

**macOS**: 安装 [MacTeX](https://www.tug.org/mactex/)

**Linux**: 
```bash
sudo apt-get install texlive-full
```

## 🔐 加密论文解密方法

第一篇论文已加密保护:

**文件**: `thesis_encrypted.zip`  
**密码**: `manbo2025`

### 解密方法:

**Windows**:
- 右键点击ZIP文件 → 解压缩
- 输入密码: `manbo2025`

**macOS**:
- 双击ZIP文件
- 输入密码: `manbo2025`

**Linux**:
```bash
unzip -P manbo2025 thesis_encrypted.zip
```

## 📝 论文选择建议

### 如果您的毕业设计侧重于...

**综合性项目**: 选择第1篇 (综合版)
- 涵盖硬件、软件、算法各方面
- 结构完整,内容全面

**算法研究**: 选择第2篇 (算法与控制)
- 深入分析PWM控制、步态规划、传感器融合等算法
- 包含详细的数学推导和实验分析

**嵌入式开发**: 选择第3篇 (嵌入式系统)
- 详细介绍STM32外设配置和驱动开发
- 包含完整的代码示例和调试方法

### 如果您需要...

**快速提交**: 使用第1篇PDF (已编译好)

**自定义修改**: 使用Markdown或LaTeX源文件

**多角度参考**: 三篇论文可以互相补充

## 🎓 使用建议

1. **选择合适的论文**: 根据您的毕业设计要求选择最合适的论文

2. **修改个人信息**: 在论文中填写您的个人信息(姓名、学号、班级、指导教师等)

3. **调整内容**: 根据实际项目情况调整论文内容

4. **检查格式**: 确保论文格式符合学校要求

5. **查重检测**: 提交前进行查重检测

## 📊 论文统计

| 论文 | 字数 | 章节数 | 页数(估算) | 格式 |
|------|------|--------|------------|------|
| 综合版 | 27,000 | 7 | 60-70 | PDF + MD + ZIP |
| 算法控制版 | 25,000 | 7 | 55-65 | MD + TEX |
| 嵌入式开发版 | 22,000 | 7 | 50-60 | MD + TEX |

## 🔗 相关文档

- [项目README](../README.md) - 项目总体说明
- [硬件设计文档](hardware.md) - 硬件设计详细说明
- [软件设计文档](software.md) - 软件设计详细说明
- [4舵机配置说明](4servo_config.md) - 4舵机专用配置
- [代码示例文档](code_examples.md) - 实际代码示例

## ⚠️ 注意事项

1. **学术诚信**: 论文仅供参考,请根据实际项目情况修改,切勿直接抄袭

2. **版权声明**: 论文内容基于开源项目,遵循MIT协议

3. **个人信息**: 请务必修改论文中的个人信息部分

4. **格式要求**: 不同学校对论文格式要求不同,请根据学校要求调整

5. **查重率**: 建议进行查重检测,确保符合学校要求

## 📞 技术支持

如有问题,请在GitHub仓库提交Issue:
https://github.com/RussellCooper-DJZ/manbo-robot-dog/issues

---

**更新日期**: 2025-10-28  
**项目地址**: https://github.com/RussellCooper-DJZ/manbo-robot-dog

