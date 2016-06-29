#STM32F4 Studying Notes

####FSMC
FSMC(Flexible Static Memory Controller，可变静态存储控制器)是STM32系列采用一种新型的存储器扩展技术。在外部存储器扩展方面具有独特的优势，可根据系统的应用需要，方便地进行不同类型大容量静态存储器的扩展。  
是STM32系列中内部集成256 KB以上FlaSh，后缀为xC、xD和xE的高存储密度微控制器特有的存储控制机制。之所以称为“可变”，是由于通过对特殊功能寄存器的设置，FSMC能够根据不同的外部存储器类型，发出相应的数据/地址/控制信号类型以匹配信号的速度，从而使得STM32系列微控制器不仅能够应用各种不同类型、不同速度的外部静态存储器，而且能够在不增加外部器件的情况下同时扩展多种不同类型的静态存储器，满足系统设计对存储容量、产品体积以及成本的综合要求。

####硬件初始化
启动代码： 启动代码是一段和硬件相关的汇编代码。 是必不可少的！ 这代码主要作用如
下： 1、堆栈（ SP）的初始化； 2、初始化程序计数器（ PC）； 3
、设置向量表异常事件的入口地
址； 4、调用 main 函数。  
ST 公司为 STM32F40x 和 STM32F41x 系列的 STM32F4 提供了一个共同的启动文件，名字
为： startup_stm32f40_41xxx.s。

####AHB总线
由ARM公司推出的AMBA片上总线受到了广大IP开发商和SoC系统集成者的青睐，已成为一种流行的工业标准片上结构。AMBA规范主要包括了AHB(Advanced High performance Bus)系统总线和APB(Advanced Peripheral Bus)外围总线。  
- Advanced High-performance Bus (AHB)
AHB总线用于高性能，高时钟工作频率模块。AHB在AMBA架构中为系统的高性能运行起到了基石作用。AHB为高性能处理器，片上内存，片外内存提供接口，同时桥接慢速外设。
- Advanced System Bus (ASB)
ASB总线主要用于高性能系统模块。ASB是可用于AHB不需要的高性能特性的芯片设计上可选的系统总线。ASB也支持高性能处理器，片上内存，片外内存提供接口和慢速外设。
- Advanced Peripheral Bus (APB)
APB总线用于为慢速外设提供总线技术支持。
APB是一种优化的，低功耗的，精简接口总线，可以技术多种不同慢速外设。由于APB是ARM公司最早提出的总线接口，APB可以桥接ARM体系下每一种系统总线。  

####SysTick
```C 
typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type;

```
SysTick是MDK定义的一个结构体, 里边包含了CTRL, LOAD, VAL, CALIB四个寄存器.  
在`delay.c`中:
```C
SysTick->CTRL&=~(1<<2);					//SYSTICK使用外部时钟源	
	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使用
```
原语句相当于`SysTick->CTRL&=0xfffffffb`也就相当于把`CTRL`寄存器第3位置0, 根据`CTRL`寄存器各位定义, 相当于选择了`HCLK/8`(也就是CPU时钟频率的1/8)为SysTick频率.  
```C
static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
```
fac_us，为 us 延时的基数，也就是延时 1us， SysTick->LOAD 所应
设置的值。   
 Systick 的时钟来自系统时钟 8 分频， 正因为如此，
系统时钟如果不是 8 的倍数(不能被 8 整除)，则会导致延时函数不准确.


####存储器系统
CM3 的存储器系统支持所谓的“位带”（ bit‐band）操作。通过它，实现了对单
一比特的原子操作。  
支持了位带操作后，可以使用普通的加载/存储指令来对单一的比特进行读写。

在位带区中，每个比特都映射到别名地址区的一个字——这是只有 LSB 有效的字。当一个别名地址被访问时，会先把该地址变换成位带地址。对于读操作，读取位带地址中的一个字，再把需要的位右移到 LSB，并把 LSB 返回。对于写操作，把需要写的位左移至对应的位序号处，然后执行一个原子的“读－改－写”过程。  
注意:当使用位带功能时, 要访问的变量必须用`volatile`来定义. 因为C编译器不知道同一个比特可以有两个地址. 






















