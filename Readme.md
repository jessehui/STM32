#STM32F4 Studying Notes

>使用正点原子STM32F4开发板


##PART1 寄存器开发

####FSMC
FSMC(Flexible Static Memory Controller，可变静态存储控制器)是STM32系列采用一种新型的存储器扩展技术。在外部存储器扩展方面具有独特的优势，可根据系统的应用需要，方便地进行不同类型大容量静态存储器的扩展。  
是STM32系列中内部集成256 KB以上FlaSh，后缀为xC、xD和xE的高存储密度微控制器特有的存储控制机制。之所以称为“可变”，是由于通过对特殊功能寄存器的设置，FSMC能够根据不同的外部存储器类型，发出相应的数据/地址/控制信号类型以匹配信号的速度，从而使得STM32系列微控制器不仅能够应用各种不同类型、不同速度的外部静态存储器，而且能够在不增加外部器件的情况下同时扩展多种不同类型的静态存储器，满足系统设计对存储容量、产品体积以及成本的综合要求。

####常用置位操作
```C
x |= (1<<y);	//x的第y位置1(y可以等于0)
x &= ~(1<<y);	//x的第y位清0

x |= (3<<y);	//x的第y+1和第y位置11
```


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

####sys.h
`FTIR`下降沿触发方式(Falling edge Trigger).  
`RTIR`上升沿触发方式(Raising edge Trigger).

推挽输出和开漏输出
推挽输出:可以输出高,低电平,连接数字器件;  

开漏输出:输出端相当于三极管的集电极. 要得到高电平状态需要上拉电阻才行. 适合于做电流型的驱动,其吸收电流的能力相对强(一般20ma以内).  
推挽结构一般是指两个三极管分别受两互补信号的控制,总是在一个三极管导通的时候另一个截止.
场效应管是电压控制型元器件，只要对栅极施加电压，DS就会导通。
结型场效应管有一个特性就是它的输入阻抗非常大，这意味着：没有电流从控制电路流出，也没有电流进入控制电路。没有电流流入或流出，就不会烧坏控制电路。而双极型晶体管不同，是电流控制性元器件，如果使用开集电路，可能会烧坏控制电路。这大概就是我们总是听到开漏电路而很少听到开集电路的原因吧？因为开集电路被淘汰了。  
NVIC: 嵌套向量中断控制器(Nested Vectored Interrupt Controller)。STM32的中有一个强大而方便的NVIC，它是属于Cortex内核的器件，不可屏蔽中断 (NMI)和外部中断都由它来处理，而SYSTICK不是由 NVIC来控制的。   
STM32中有两个优先级的概念——抢占式优先级和响应优先级，有人把响应优先级称作'亚优先级'或'副优先级'，每个中断源都需要被指定这两种优先级。

Thumb指令可以看做是ARM指令压缩形式的子集，是针对代码密度的问题而提出的，它具有16为的代码密度。Thumb不是一个完整的体系结构，不能指望处理程序只执行Thumb指令而不支持ARM指令集。因此，Thumb指令只需要支持通用功能，必要时，可借助完善的ARM指令集，例如：所有异常自动进入ARM状态。



####锁相环
PLL(Phase Locked Loop)作用:数字芯片有个时钟树的概念，现在比如就是一根导线代替锁相环，芯片外面在时钟的上升沿开始给芯片送入一组数据，芯片内部由于有时钟树的存在，导致了内部时序电路实际使用的时钟是延迟过的，进而产生一个数据漂移的现象。但是有锁相环了，我们可以把时钟树的其中一个分支接入锁相环，使时钟树末梢的相位频率与参考信号保持一致，就不会有数据漂移的现象了。  
以上是锁相环最简单的使用，锁相环还有倍频作用，因为输出的时钟是它自己内部的压控振荡器产生的，若加一个分频器，再与输入参考时钟相比较，就可得到一个频率加N倍的时钟信号，当然相位还是和参考时钟是同步的。
RCC: Reset & Clock Controller

```c
//系统时钟初始化函数
//plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	RCC->CR|=0x00000001;		//设置HISON,开启内部高速RC振荡
	RCC->CFGR=0x00000000;		//CFGR清零 
	RCC->CR&=0xFEF6FFFF;		//HSEON,CSSON,PLLON清零 
	RCC->PLLCFGR=0x24003010;	//PLLCFGR恢复复位值 
	RCC->CR&=~(1<<18);			//HSEBYP清零,外部晶振不旁路
	RCC->CIR=0x00000000;		//禁止RCC时钟中断 
	Sys_Clock_Set(plln,pllm,pllp,pllq);//设置时钟 
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(1<<29,0x0);
#else   
	MY_NVIC_SetVectorTable(0,0x0);
#endif 
}		    
```

####时钟控制
在STM32中，有五个时钟源，为HSI、HSE、LSI、LSE、PLL。 其实是四个时钟源，如下图所示（灰蓝色），PLL是由锁相环电路倍频得到PLL时钟。  
1. HSI是高速内部时钟，RC振荡器，频率为8MHz。
2. HSE是高速外部时钟，可接石英/陶瓷谐振器，或者接外部时钟源，频率范围为4MHz~16MHz。
3. LSI是低速内部时钟，RC振荡器，频率为40kHz。
4. LSE是低速外部时钟，接频率为32.768kHz的石英晶体。
5. PLL为锁相环倍频输出，其时钟输入源可选择为HSI/2、HSE或者HSE/2。倍频可选择为2~16倍，但是其输出频率最大不得超过72MHz。
STM32F4 的时钟设计的比较复杂，各个时钟基本都是可控的，任何外设都有对应的时钟控制开关，这样的设计，对降低功耗是非常有用的，不用的外设不开启时钟，就可以大大降低其功耗。
时钟设置函数:  
`u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)`   
- pllm: 主PLL和音频PLL分频系数(PLL之前的分频)
- plln: 主PLL分频系数(PLL倍频)
- pllp: 系统时钟的主PLL分频系数(PLL之后的分频)
- pllq: USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
VCO即压控振荡器，是射频电路的重要组成部分。 射频电路多采用调制解调方式，因此严重依赖本振。



####内联汇编
THUMB指令不支持汇编内联,采用如下方法实现执行汇编指令WFI .  
```C++
//(sys.c中)
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//__ASM宏定义(core_cm4.h中)
#if   defined ( __CC_ARM )
#define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */  
//...

#elif defined ( __GNUC__ )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */   
//...
```
在执行完 WFI 指令之后， STM32F4 就进入待机模式了，系统将停止工作

####IO设置函数
STM32F4 每组通用 I/O 端口包括 4 个 32 位配置寄存器（ MODER、 OTYPER、 OSPEEDR和 PUPDR）、 2 个 32 位数据寄存器（ IDR 和 ODR）、 1 个 32 位置位/复位寄存器 (BSRR)、1 个 32 位锁定寄存器 (LCKR) 和 2 个 32 位复用功能选择寄存器（ AFRH 和 AFRL）等。  

- `MODER`寄存器: GPIO端口模式控制寄存器. 每组IO下有16个IO口, 该寄存器有32位, 每2个位控制1个IO.   
`00`:输入(复位状态); `01`:通用输出模式;`10`:复用功能模式. `11`:模拟模式  
- `OTYPER`寄存器: 控制GPIO的输出类型.该寄存器仅用于输出模式, 在输入模式(MODER[1:0]=00/11时)不起作用. 该寄存器仅低16位有效, 每位控制1个IO口,复位后,各位均为0.   
`0`: 输出推挽; `1`: 输出开漏. 
- `OSPEEDR`寄存器: 用于控制GPIO的输出速度. 仅用于输出模式. 每两个位控制1个IO口.   
`00`: 2MHz; `01`: 25MHz; `10`: 50MHz; `11`: 100MHz或80MHz
- 'PUPDR'寄存器: 用于控制GPIO的上下拉. 每2位控制一个IO口. STM32F1通过`ODR`寄存器控制上下拉的, 而STM32F4则由单独的寄存器PUPDR控制上下拉,使用更灵活.   
`00`: 无上下拉; `01`: 上拉; `10`: 下拉; `11`: 保留.
- `ODR`寄存器: 控制GPIO的输出. 用于设置某个IO输出低电平还是高电平.也仅在输出模式下有效.每1位控制1个IO口, 1位输出高, 0为输出低. 
- `IDR`寄存器: 读取GPIO的输入. 用于读取某个IO的电平, 高16位保留, 低16位每个位对应一个IO,1表示该IO输入的是高电平, 0表示输入的是低电平.   

例如: 设置PortC的第12个IO(PC11)为推挽输出,速度为100MHz, 不带上下拉, 并输出高电平. 则寄存器配置代码如下:
```C
RCC->AHB1ENR|=1<<2; //使能 PORTC 时钟
GPIOC->MODER&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC->MODER|=1<<(11*2); //设置 PC11 为输出模式
GPIOC->OTYPER&=~(1<<11) ; //清除 PC11 原来的设置
GPIOC->OTYPER|=0<<11; //设置 PC11 为推挽输出
GPIOC-> OSPEEDR&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC-> OSPEEDR|=3<<(11*2); //设置 PC11 输出速度为 100Mhz
GPIOC-> PUPDR&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC-> PUPDR|=0<<(11*2); //设置 PC11 不带上下拉
GPIOC->ODR|=1<<11; //设置 PC11 输出 1（高电平）

```
所以要使用某个IO口,首先确定是哪个Port的第几个脚. 然后使能`AHB1ENR`寄存器的对应位. 然后配置GPIO对应Port的寄存器来控制某一位

除此之外, 还有两个寄存器`AFRL`和`AFRH`, 它们是用来设置IO引脚复用和映射的. STM32F4每个IO引脚通过一个复用器连接到板载外设/模块,该复用器一次仅允许一个外设的复用功能(Alternat Function)连接到IO引脚.复用功能选择,就是通过AFRL和AFRH来控制的. AFRL控制0~7IO, AFRH控制8~15IO.

AFRL和AFRH被定义为`AFR[2]`. `AFR[0]`代表AFRL, `AFR[1]`代表AFRH.  
U3_RX(串口3的接收引脚), 在`PB11/PC11/PD9`上面都有这个复用功能, 这就要通过对应IO的复用功能选择器来选择,可以选择3个脚里任意一个作为U3_RX,只需要设置对应引脚所在GPIO的AFRL/AFRH即可,而且,没有选择作为 U3_RX 复用的另外两个 IO 口，还是可以用来作为普通 IO 输出或者其他复用功能输出的。


####中断管理
CM4内核支持256个中断, 其中包含16个内核中断和240个外部中断. 并且具有 256级的可编程中断设置.  
STM32F40XX/STM32F41XX只是用了CM4内核的一部分,共有92个中断.包括10个内核中断和82个可屏蔽中断. 具有16级可编程的中断优先级.
抢占优先级就是假如当前情况是在运行着某个中断程序的情况先，触发了一个中断信号，而且比当前的中断等级要高，那么当前的中断程序会被挂起，直接跳到高抢占优先级的中断程序去。一般说法就是：具有高抢占式优先级的中断可以在具有低抢占式优先级的中断处理过程中被响应，即中断嵌套，或者说高抢占式优先级的中断可以嵌套低抢占式优先级的中断。  
响应优先级就是来一个中断运行一个中断程序，如果两个中断信号来到，并且抢占优先级相同，那么判断响应优先级高的先运行，结束后再运行优先级低的。而这运行中断程序当中再来同抢占优先级，不同响应优先级，是不会打断当前运行的程序，也只会等到当前中断程序运行完后再运行。即这两个中断没有任何嵌套关系。  

NVIC: Nested Vectored Interrupt Controller   
嵌套向量中断控制器  
ISER[8] 全称是： Interrupt Set-Enable Registers，这是一个中断使能寄存器组  
`__IO uint32_t ISER[8];		 /*!< Interrupt Set Enable Register */`    
CM4内核支持256个中断,这里用8个32位寄存器来控制. 每个位控制一个中断. 但是STM32F40XX可屏蔽中断只有82个,所以有用的就是`ISER[0~2]`, 共可以表示96个中断.但只用了82个. 0~81分别对应ISER[0]0~31位,ISER[1]0~31,ISER[2]0~17.要使能某个中断,必须设置相应的ISER位为1.   
ICER 全称: Interrupt Clear-Enable Registers. 用来清除某个中断的使能的. 写1有效.    
ISPR 全称: Interrupt Set-Pending Registers 中断挂起控制寄存器组. 通过置1,可以将正在进行的中断挂起, 而去执行同级或者更高级的中断.   
ICPR: 中断解挂.置1有效  
IABR:中断激活标志位寄存器组(Interrupt Active Bit Register). 只读寄存器, 通过它可以知道当前在执行的中断是哪一个(置1). 中断执行完后由硬件清0.  
前边的寄存器的对应位和ISER是一样的.
IP[240](Interrupt Priority Registers)是一个中断优先级控制的寄存器组. STM32中断分组与这个寄存器组密切相关.  
IP寄存器组由240个8bit寄存器组成.每个可屏蔽中断占用8bit. 这样可以总共表示240个可屏蔽中断.而STM32F40XX只用了其中的82个,IP[81]~IP[0]分别对应中断81~0. 而每个可屏蔽中断占用的8bit并未全部使用. 而是只用了高4位.

STM32F4中断分组: 这里的中断分组指的是把每个中断优先级分成2种,抢占优先级和响应优先级后, 要给每个中断的2种优先级分配bit数.一共5种分配方式所以共分5组,组0~4. 分组设置由`SCB->AIRCR`寄存器的`bit10~8`定义.  
SCB: System Control Block register  
AIRCR: Application Interrupt and Reset Control Register  
系统软复位也用到此寄存器:
```C
void Sys_Soft_Reset(void)
{
	SCB->AIRCR = 0x05FA0000 | (u32)0x04;
}
```
要实现STM32F4的软复位, 只要置位`Bit2`, 这样就可以请求一次软复位. __但是__ ,该寄存器的Bit31~16为访问钥匙, 要将访问钥匙`0X05FA0000`与我们要进行的操作相`或`,然后写入`AIRCR`, 这样才被CM4接受.


组 	| AIRCR[10:8] 	| IP[7:4]分配情况 | 分配结果 						   
-------|---------------|------------------|-----------------------------------
0		| 111		 	| 	0:4 		   | 0 位抢占优先级， 4 位响应优先级   
1	 	| 110			|   1:3 		   | 1 位抢占优先级， 3 位响应优先级   
2	 	| 101	 		|   2:2   		   | 2 位抢占优先级， 2 位响应优先级   
3	 	| 100			|   3:1 		   | 3 位抢占优先级， 1 位响应优先级   
4	 	| 011			|   4:0			   | 4 位抢占优先级， 0 位响应优先级   

例如: 组设置为3, 那么此时所有82个中断, 每个中断的中断优先寄存器(`IP`)的高四位中的最高3位是抢占优先级, 低1位是响应优先级. 每个中断抢占优先级可以被设为0~7(因为有3位), 响应优先级可以被设为1或0. 抢占优先级高于响应优先级, 数值越小所代表的优先级越高.   
注: (1)如果两个中断的抢占优先级和响应优先级都一样的话, 则看哪个中断先发生  
(2)高优先级的抢占优先级是可以打断正在进行的低抢占优先级中断的. 但是高优先级的响应优先级不可以打断低响应优先级的中断

中断配置:
我们知道 SCB->AIRCR 的修改需要通过在高 16 位写入 0X05FA这个密钥才能修改的，故在设置 AIRCR 之前，应该把密钥加入到要写入的内容的高 16 位，以保证能正常的写入 AIRCR。在修改 AIRCR 的时候，我们一般采用读改写的步骤，来实现不改变 AIRCR 原来的其他设置。

NVIC设置参数函数:`MY_NVIC_Init`. 该函数有4个参数, NVIC_PreemptionPriority(抢占优先级), NVIC_SubPriority(响应优先级), NVIC_Channel(中断编号), NVIC_Group(中断分组0~4). __注意优先级不能超过设定的组的范围__. 优先级原则是数值越小, 越优先.  
  

####外部中断
STM32F4的EXTI控制器(External Interrupt/Event Controller)支持23个外部中断事件请求. 每个中断设有状态位, 每个中断/事件都有独立的触发和屏蔽设置.
```C
/** 
  * @brief External Interrupt/Event Controller
  */

typedef struct
{
  __IO uint32_t IMR;    /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
  __IO uint32_t EMR;    /*!< EXTI Event mask register,                Address offset: 0x04 */
  __IO uint32_t RTSR;   /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
  __IO uint32_t FTSR;   /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
  __IO uint32_t SWIER;  /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
  __IO uint32_t PR;     /*!< EXTI Pending register,                   Address offset: 0x14 */
} EXTI_TypeDef;

```
IMR: 中断屏蔽寄存器. 32位但只有前23位有效. 每位控制一个外部中断. 置1开启中断, 否则关闭.  
EMR: 事件屏蔽寄存器. 同IMR  
RTSR: 上升沿触发选择寄存器.同IMR. 位X对应线X上的上升沿触发. 如果设置为1, 则是允许上升沿触发中断/事件. 否则不允许.  
FTSR: 下降沿触发选择寄存器. 同RTSR.  
PR: 挂起寄存器. 当外部中断线上发生了选择的边沿事件, 该寄存器的对应位会被置为1. 对应线上没有发生触发请求时为0. 通过向该寄存器特定位写1来清除该位. 在中断服务函数里面经常会要向该寄存器的对应位写1来清除中断请求. 
SWIER: 软件中断事件寄存器. 在未设置IMR和EMR时, 通过向该寄存器的x位写1, 将设置PR中相应位挂起. 如果设置了IMR,EMR将产生一次中断.   
外部 IO 口的中断，还需要一个寄存器配置，也就是外部中断配置寄存器 `EXTICR`。这是因为 STM32F4 任何一个 IO 口都可以配置成中断输入口，但是 IO 口的数目远大于中断线数（ 16 个）。于是 STM32F4 就这样设计，GPIOA~GPIOI的[15： 0]分别对应中断线 15~0。这样每个中断线对应了最多 9 个 IO 口(A~I). 所以一共有16*9个选项. 而中断线每次只能连接到 1 个 IO 口上，这样就需要 EXTICR 来决定对应的中断线配置到哪个GPIO 上了.  
```C
/** 
  * @brief System configuration controller
  */
  
typedef struct
{
  __IO uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
  __IO uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
  __IO uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
  uint32_t      RESERVED[2];  /*!< Reserved, 0x18-0x1C                                                          */ 
  __IO uint32_t CMPCR;        /*!< SYSCFG Compensation cell control register,         Address offset: 0x20      */
} SYSCFG_TypeDef;

```  
EXTICR寄存器在SYSCFG结构体中定义. 可以看到EXTICR寄存器组总共有4个, 每个32位, 但只用了其低16位. 每个EXTICR寄存器的低16位被以4位分成一组, 这样就可以配置4*4=__16__个引脚(15~0). 然后每一组有4位, 可以用来选择PA~PI(9个).


####1_LED
同时配置两个IO口控制LED.  
```C
GPIO_Set(GPIOF,PIN9|PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,
GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10 设置
```
在配置 STM32F4 外设的时候，任何时候都要先使能该外设的时钟！AHB1ENR 是 AHB1 总线上的外设时钟使能寄存器

```C
#define LED0 PFout(9) // DS0
#define LED1 PFout(10) // DS1
```
这里使用的是位带操作来实现操作某个 IO 口的.

```C
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
//...
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
```
或者也可以使用另一种方法, 此方法对处理器依赖更小.  
```C
#define LED0 (1<<9) //led0 PF9
#define LED1 (1<<10) //led1 PF10
#define LED0_SET(x) GPIOF->ODR=(GPIOF->ODR&~LED0)|(x ? LED0： 0)
#define LED1_SET(x) GPIOF->ODR=(GPIOF->ODR&~LED1)|(x ? LED1： 0)
```


####IAP 和 ISP
IAP: In Application Programming，IAP是用户自己的程序在运行过程中对User Flash的部分区域进行烧写，目的是为了在产品发布后可以方便地通过预留的通信口对产品中的固件程序进行更新升级。  
通常在用户需要实现IAP功能时，即用户程序运行中作自身的更新操作，需要在设计固件程序时编写两个项目代码，第一个项目程序不执行正常的功能操作，而只是通过某种通信管道(如USB、USART)接收程序或数据，执行对第二部分代码的更新；第二个项目代码才是真正的功能代码。这两部分项目代码都同时烧录在User Flash中，当芯片上电后，首先是第一个项目代码开始运行，它作如下操作：
(1) 检查是否需要对第二部分代码进行更新  
(2) 如果不需要更新则转到(4)  
(3) 执行更新操作  
(4) 跳转到第二部分代码执行  
第一部分代码必须通过其它手段，如JTAG或ISP烧入；第二部分代码可以使用第一部分代码IAP功能烧入，也可以和第一部分代码一道烧入，以后需要程序更新是再通过第一部分IAP代码更新。  

ISP: In System Program 在系统编程;
用写入器将code烧入,不过,芯片可以在目标板上,不用取出来,在设计目标板的时候就将接口设计在上面,所以叫"在系统编程",即不用脱离系统。已经编程的器件也可以用ISP方式擦除或再编程，ISP技术是未来发展方向。
与ISP对应的有IAP,即In Applicatin Programming在应用编程；


####蜂鸣器使用
蜂鸣器采用直流电压供电.分为压电式和电磁式两种. 又分为有源和无源两种,有源表示带有自当震荡电路, 一通电就会发声. 无源蜂鸣器不带振荡电路, 必须外部提供2~5KHz左右的方波驱动,才能发声.  
能否直接用 STM32 的 IO 口驱动呢？ 让我们来分析下：  
STM32F4 的单个 IO 最大可以提供 25mA 电流（来自数据手册），而蜂鸣器的驱动电流是 30mA左右，两者十分相近，但是全盘考虑， STM32F4 整个芯片的电流， 最大也就 150mA，如果用IO 口直接驱动蜂鸣器，其他地方用电就得省着点了…所以，我们不用 STM32F4 的 IO 直接驱动蜂鸣器，而是通过三极管扩流后再驱动蜂鸣器，这样 STM32F4 的 IO 只需要提供不到 1mA 的电流就足够了。

注意使用位带操作(Bitband)只能在使用端口`PORTA~I`中.


####串口通信
`USART1_IRQHandler()`函数是串口1的中断响应函数. 当串口1发生了相应的中断,就会跳到该函数执行. 
对于`uart_init()`函数, 由于STM32F4采用了分数波特率, 所以STM32F4的串口波特率设置范围很宽, 而且误差很小.
每个串口都有一个自己独立的波特率寄存器USART_BRR, 通过设置该寄存器就可以达到配置不同波特率的目的.

几个与串口基本配置直接相关的寄存器:
1. 串口时钟使能. 串口作为外设, 由APB2ENR寄存器控制  
2. 串口波特率设置. 每个串口都有一个自己独立的波特率寄存器USART_BRR  
3. 串口控制. 每个串口都有3个控制寄存器USART_CR1~3.
4. 数据发送与接收. 通过数据寄存器USART_DR. 这是一个双寄存器, 包含了TDR,RDR
5. 串口状态. 可以通过状态寄存器USART_SR读取.   

`!`和`~`的区别:  
按位取反“~”： 按位取反1变0，0变1  
逻辑 非“！”： 逻辑取反, false变true,true变false， 在C中，只要不是0就是真

> 自定义的`USART_RX_STA`寄存器定义表

bit15 | bit14 | bit13~0
-------|------|---------
接收完成标志| 接收到0X0D标志| 接收到的有效数据个数


####中断控制
要把IO口作为外部中断输入, 有以下几个步骤:
(1) 初始化IO口为输入:  
以设置为上拉/下拉输入，也可以设置为浮空输入，但浮空的时候外部一定要带上拉，或者下拉电阻。否则可能导致中断不停的触发。在干扰较大的地方，就算使用了上拉/下拉，也建议使用外部上拉/下拉电阻，这样可以一定程度防止外部干扰带来的影响。
(2) 开启`SYSCFG`时钟, 设置IO口与中断线的映射关系. IO 口与中断线的对应关系需要配置外部中断配置寄存器 `EXTICR`. 该寄存器在`SYSCFG`类型中.  
```C
typedef struct
{
  __IO uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
  __IO uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
  __IO uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
  uint32_t      RESERVED[2];  /*!< Reserved, 0x18-0x1C                                                          */ 
  __IO uint32_t CMPCR;        /*!< SYSCFG Compensation cell control register,         Address offset: 0x20      */
} SYSCFG_TypeDef;
```
(3) 开启与该IO口相对的线上中断, 设置触发条件.
要配置中断产生的条件， STM32F4 可以配置成上升沿触发，下降沿触发，或者任意电平变化触发，但是不能配置成高电平触发和低电平触发。
(4) 配置中断分组(NVIC), 并使能中断.
(5) 编写中断服务函数. 

STM32F4 的外部中断 0~4 都有单独的中断服务函数，但是从 5 开始，他们就没有单独的服务函数了，而是多个中断共用一个服务函数，比如外部中断 5~9 的中断服务函数为： void EXTI9_5_IRQHandler(void)，类似的， void EXTI15_10_IRQHandler(void)就是外部中断 10~15 的中断服务函数。
```assembly
DCD     EXTI0_IRQHandler                  ; EXTI Line0                                             
DCD     EXTI1_IRQHandler                  ; EXTI Line1                                             
DCD     EXTI2_IRQHandler                  ; EXTI Line2                                             
DCD     EXTI3_IRQHandler                  ; EXTI Line3                                             
DCD     EXTI4_IRQHandler                  ; EXTI Line4        

DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s   

DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s  
```



####独立看门狗(IWDG)
STM32F4自带2个看门狗: 独立看门狗(IWDG)和窗口看门狗(WWDG).独立看门狗由内部专门的32KHz低速时钟驱动(LSI,是一个内部RC时钟,并不是准确的32KHz,而是15~47KHz中某个值,看门狗对时间的要求不是很精确,这样是可以的). 

相关寄存器:  
- 键值寄存器(IWDG_KR): 必须每隔一段时间通过软件对这些位写入键值`AAAAh`. 否则当计数器到0时,会产生复位. 写入键值`5555h`可使能对IWDG_PG和IWDG_RLR寄存器的访问. 写入键值`CCCCh`可启动看门狗.
- 状态寄存器(IWDG_SR): 32位寄存器,只用了最低2位. 
	- BIT1: RVU(reload value update)计数器重载值更新, 重载值只有在RVU为0时才可更新.
	- BIT0: PVU(prescaler value update)分频器值更新.
- 重装载寄存器(IWDG_RLR): 用来保存重装到计数器中的值. 这个值由软件设置, 每次对IWDG_KR寄存器写入值AAAAh时, 这个值就会重装载到看门狗计数器中. 之后, 看门狗计数器便从该装载值开始递减计数.
- 预分频寄存器(IWDG_PR): 用来设置看门狗时钟的分频系数. 最低为4,最高为256. 该寄存器是一个32位寄存器,但是我们只用了最低3位.

注: IWDG_PR 和 IWDG_RLR 寄存器具有写保护功能。要修改这两个寄存器的值，必须先向
IWDG_KR 寄存器中写入 0x5555。 

启动STM32F4的独立看门狗, 启动过程:
(1) 向IWDG_KR写入0X5555. 
通过这步，我们取消 IWDG_PR 和 IWDG_RLR 的写保护，使后面可以操作这两个寄存器。  
(2) 设置 IWDG_PR 和 IWDG_RLR 的值。
这两步设置看门狗的分频系数，和重装载的值。由此，就可以知道看门狗的喂狗时间（也就是看门狗溢出时间），该时间的计算方式为：  
Tout=((4×2^prer) ×rlr) /32  
其中 Tout 为看门狗溢出时间（单位为 ms）； prer为看门狗时钟预分频值（ IWDG_PR 值），范围为 0~7； rlr 为看门狗的重装载值（ IWDG_RLR 的值）；比如我们设定 prer 值为 4， rlr 值为 500，那么就可以得到 Tout=64×500/32=1000ms，这样，看门狗的溢出时间就是 1s，只要你在一秒钟之内，有一次写入 0XAAAA 到 IWDG_KR，就不会导致看门狗复位（当然写入多次也是可以的）。  
(3) 向 IWDG_KR 写入 0XAAAA。  
通过这句，将使 STM32F4 重新加载 IWDG_RLR 的值到看门狗计数器里面。 即实现独立看门狗的喂狗操作。  
(4) 向 IWDG_KR 写入 0XCCCC。  
通过这句，来启动 STM32F4 的看门狗。 注意 IWDG 在一旦启用，就不能再被关闭！想要关闭，只能重启，并且重启之后不能打开 IWDG，否则问题依旧，所以在这里提醒大家，如果不用 IWDG 的话，就不要去打开它，免得麻烦。


####窗口看门狗(WWDG)
窗口看门狗（ WWDG）通常被用来监测由外部干扰或不可预见的逻辑条件造成的应用程序背离正常的运行序列而产生的软件故障。除非递减计数器的值在 T6 位 （ WWDG->CR 的第六位）变成 0 前被刷新，看门狗电路在达到预置的时间周期时，会产生一个 MCU 复位。在递减计数器达到窗口配置寄存器(WWDG->CFR)数值之前，如果 7 位的递减计数器数值(在控制寄存器中)被刷新， 那么也将产生一个 MCU 复位。这表明递减计数器需要在一个有限的时间窗口中被刷新。  
窗口看门狗，所以称之为窗口就是因为其喂狗时间是一个有上下限的范围内，你可以通过设定相关寄存器，设定其上限时间和下限时间。喂狗的时间不能过早也不能过晚。
T[6:0]就是 WWDG_CR 的低七位， W[6:0]即是 WWDG->CFR 的低七位。T[6:0]
就是窗口看门狗的计数器，从它定义的数值开始递减, 而 W[6:0]则是窗口看门狗的上窗口，下窗口值是固定的（ 0X40）。  
窗口看门狗的超时公式如下：  
Twwdg=(4096×2^WDGTB×(T[5:0]+1)) /Fpclk1;  
其中：  
- Twwdg： WWDG 超时时间（单位为 ms）  
- Fpclk1： APB1 的时钟频率（单位为 Khz）  
- WDGTB： WWDG 的预分频系数  
- T[5:0]：窗口看门狗的计数器低 6 位  

相关寄存器:  
- WWDG_CR: 控制寄存器. 0~6位是T[6:0] 用来存储看门狗的计数器值. 当该寄存器的值从0x40变为0x3F时, 将产生看门狗复位. 还有第7位WDGA是看门狗的激活位, 该位由软件置1. 以启动看门狗.
- WWDG_CFR: 配置寄存器. EWI(Early Wakeup Interrupt), 置1后, 只要计数器值达到0x40就会产生中断.我们一般用该位来设置中断，当窗口看门狗的计数器值减到 0X40的时候，如果该位设置，并开启了中断，则会产生中断，我们可以在中断里面向 WWDG_CR 重新写入计数器的值，来达到喂狗的目的。注意这里在进入中断后， 必须在不大于 1 个窗口看门狗计数周期的时间内重新写 WWDG_CR，否则，看门狗将产生复位.  
- WWDG_SR: 状态寄存器. 该寄存器用来记录当前是否有提前唤醒的标志。该寄存器仅有位 0 有效，其他都是保留位。当计数器值达到 40h 时，此位由硬件置 1。它必须通过软件写 0 来清除。对此位写 1 无效。 即使中断未被使能， 在计数器的值达到 0X40的时候， 此位也会被置 1。

使用WWDG:
1. 使能WWDG时钟. 不同于IWDG拥有自己独立的32KH时钟, WWDG使用的是PCLK1的时钟, 需要先使能时钟.
2. 设置WWDG_CFR和WWDG_CR寄存器. 包括使能窗口看门狗、开启中断、设置计数器的初始值、设置窗口值并设置分频数 WDGTB 等。
3. 开启WWDG中断并分组. 配置该中断的分组并使能. 
4. 编写中断服务函数. 通过该函数来喂狗，喂狗要快，否则当窗口看门狗计数器值减到 0X3F 的时候，就会引起软复位了。在中断服务函数里面也要将状态寄存器的 EWIF 位清空。


####定时器
STM32F4 的通用定时器包含一个 16 位或 32 位自动重载计数器（ CNT），该计数器由可编程预分频器（ PSC） 驱动。 STM32F4 的通用定时器可以被用于：测量输入信号的脉冲长度(输入捕获)或者产生输出波形(输出比较和 PWM)等。使用定时器预分频器和 RCC 时钟控制器预分频器，脉冲长度和波形周期可以在几个微秒到几个毫秒间调整。 STM32F4 的每个通用定时器都是完全独立的，没有互相共享的任何资源。  
相关寄存器:  
- 控制寄存器(TIMx_CR1):  CEN(counter enable, bit 0)位置1, 才能让定时器开始计数.  
- DMA/中断使能寄存器(TIMx_DIER): UIE(update Interrupt enable)更新中断允许位. 该位置1, 来允许由于更新事件所产生的中断.
- 预分频寄存器(TIMx_PSC): PSC[15:0]都有用. 计数器时钟频率CK_CNT等于f(ck_psc)/(PSC[15:0]+1). 用于设置对时钟进行分频,然后提供给计数器,作为计数器的时钟.
- TIMx_SMCR: 选择定时器的时钟来源. 
- TIMx_CNT: 该寄存器是定时器的计数器, 存储了当前定时器的计数值.
- TIMx_ARR: 自动重装载寄存器(Auto Reload Register).该寄存器在物理上实际对应2个寄存器.一个程序员可以直接操作的, 另外一个时看不到的, 称为影子寄存器. 事实上起作用的是影子寄存器. 根据 TIMx_CR1 寄存器中 APRE 位的设置： APRE=0 时，预装载寄存器的内容可以随时传送到影子寄存器，此时 2 者是连通的；而 APRE=1 时，在每一次更新事件（ UEV）时，才把预装载寄存器（ ARR） 的内容传送到影子寄存器. `ARR[15:0]`为设置的自动重载值, 当自动重载值为0时, 计数器不工作. 
- TIMx_SR: 状态寄存器. 用来标记当前与定时器相关的各种事件/中断是否发生. `Bit0`更新中断标志. 发生更新事件时由硬件置`1`(更新中断挂起), 但需要通过软件清`0`(未发生中断).


####STM32时钟控制
在STM32中，有五个时钟源，为HSI、HSE、LSI、LSE、PLL。  
1. HSI是高速内部时钟，RC振荡器，频率为8MHz。
2. HSE是高速外部时钟，可接石英/陶瓷谐振器，或者接外部时钟源，频率范围为4MHz~16MHz。开发板上为8MHz.
3. LSI是低速内部时钟，RC振荡器，频率为40kHz。
4. LSE是低速外部时钟，接频率为32.768kHz的石英晶体。
5. PLL为锁相环倍频输出，其时钟输入源可选择为HSI/2、HSE或者HSE/2。倍频可选择为2~16倍，但是其输出频率最大不得超过72MHz。

其中40kHz的LSI(低速内部时钟)供独立看门狗IWDG使用，另外它还可以被选择为实时时钟RTC的时钟源。另外，实时时钟RTC的时钟源也可以选择LSE(低速外部时钟)，或者是HSE(高速外部时钟)的128分频。RTC的时钟源通过RTCSEL[1:0]来选择。

STM32中有一个全速功能的USB模块，其串行接口引擎需要一个频率为48MHz的时钟源。该时钟源只能从PLL输出端获取，可以选择为1.5分频或者1分频，也就是，当需要使用USB模块时，PLL必须使能，并且时钟频率配置为48MHz或72MHz。(armjishu.com)

另外，STM32还可以选择一个时钟信号输出到MCO脚(PA8)上，可以选择为PLL输出的2分频、HSI、HSE、或者系统时钟。

系统时钟`SYSCLK`最大频率为72MHz(STM32F1)，它是供STM32中绝大部分部件工作的时钟源。系统时钟可由PLL、HSI或者HSE提供输出，并且它通过AHB分频器分频后送给各模块使用，AHB分频器可选择1、2、4、8、16、64、128、256、512分频。其中AHB分频器输出的时钟送给5大模块使用：  
1. 送给AHB总线、内核、内存和DMA使用的HCLK时钟。
2. 分频后送给STM32芯片的系统定时器时钟（Systick=Sysclk/8=9Mhz）
3. 直接送给Cortex的自由运行时钟(free running clock)FCLK。【ARMJISHU注：FCLK 为处理器的自由振荡的处理器时钟，用来采样中断和为调试模块计时。在处理器休眠时，通过FCLK 保证可以采样到中断和跟踪休眠事件。 Cortex-M3内核的“自由运行时钟(free running clock)”FCLK。“自由”表现在它不来自系统时钟HCLK，因此在系统时钟停止时FCLK 也继续运行。FCLK和HCLK 互相同步。FCLK 是一个自由振荡的HCLK。FCLK 和HCLK 应该互相平衡，保证进入Cortex-M3 时的延迟相同。】
4. 送给APB1分频器。APB1分频器可选择1、2、4、8、16分频，其输出一路供APB1外设使用(PCLK1，低速外设, 最大频率36MHz)，另一路送给定时器(Timer)2、3、4倍频器使用。该倍频器可选择1或者2倍频，时钟输出供定时器2、3、4使用。
5. 送给APB2分频器。APB2分频器可选择1、2、4、8、16分频，其输出一路供APB2外设使用(PCLK2，高速外设, 最大频率72MHz)，另一路送给定时器(Timer)1倍频器使用。该倍频器可选择1或者2倍频，时钟输出供定时器1使用。另外，APB2分频器还有一路输出供ADC分频器使用，分频后送给ADC模块使用。ADC分频器可选择为2、4、6、8分频。

以上提到3种时钟Fclk、Hclk和Pclk，简单解释如下：  
- Fclk为供给CPU内核的时钟信号，我们所说的cpu主频为XXXXMHz，就是指的这个时钟信号，相应的，1/Fclk即为cpu时钟周期；
- Hclk为优秀的高性能总线（AHB bus peripherals）供给时钟信号（AHB为advanced high-performance bus）；HCLK ：AHB总线时钟，由系统时钟SYSCLK分频得到，一般不分频，等于系统时钟，HCLK是高速外设时钟，是给外部设备的，比如内存，flash。
- Pclk为优秀的高性能外设总线（APB bus peripherals）供给时钟信号（其中APB为advanced peripherals bus）。

在以上的时钟输出中，有很多是带使能控制的，例如AHB总线时钟、内核时钟、各种APB1外设、APB2外设等等。当需要使用某模块时，记得一定要先使能对应的时钟。

需要注意的是定时器的倍频器，当APB的分频为1时它的倍频值为1，否则它的倍频值就为2。  
连接在APB1(低速外设)上的设备有：电源接口、备份接口、CAN、USB、I2C1、I2C2、UART2、UART3、SPI2、窗口看门狗、Timer2、Timer3、Timer4。  
(注意：USB模块虽然需要一个单独的48MHz时钟信号，但它应该不是供USB模块工作的时钟，而只是提供给串行接口引擎(SIE)使用的时钟。USB模块工作的时钟应该是由APB1提供的)

连接在APB2(高速外设)上的设备有：UART1、SPI1、Timer1、
ADC1、ADC2、所有普通IO口(PA~PE)、第二功能IO口。

##库函数开发

####STM32固件库简介
任何一个做 Cortex-M4 芯片，他们的内核结构都是一样的，不同的是他们的存储器容量，片上外设， IO 以及其他模块的区别。所以不同公司设计的Cortex-M4 芯片他们的端口数量，串口数量，控制方法这些都是有区别的，这些资源他们可以根据自己的需求理念来设计。同一家公司设计的多种 Cortex-M4 内核芯片的片上外设也会有很大的区别，比如 STM32F407 和 STM32F429，他们的片上外设就有很大的区别。  
既然大家都使用的是 Cortex-M4 核，也就是说，本质上大家都是一样的，这样 ARM 公司为了能让不同的芯片公司生产的 Cortex-M4 芯片能在软件上基本兼容，和芯片生产商共同提出了一套标准 CMSIS 标准(Cortex Microcontroller Software Interface Standard) ,翻译过来是“ ARM Cortex™ 微控制器软件接口标准”。 ST 官方库就是根据这套标准设计的。  
CMSIS 分为 3 个基本功能层：
(1) 核内外设访问层： ARM 公司提供的访问，定义处理器内部寄存器地址以及功能函数。
(2) 中间件访问层:定义访问中间件的通用 API。 由 ARM 提供，芯片厂商根据需要更新。
(3) 外设访问层：定义硬件寄存器的地址以及外设的访问函数。

官方库包中的 Libraries 文件夹下面有 CMSIS 和 STM32F4xx_StdPeriph_Driver 两个目录，这两个目录包
含固件库核心的所有子文件夹和文件。Libraries 文件夹里面的文件在我们建立工程的时候都会使用到。
- CMSIS 文件夹存放的是符合 CMSIS 规范的一些文件。包括 STM32F4 核内外设访问层代码，DSP 软件库， RTOS API， 以及 STM32F4 片上外设访问层代码等。
- STM32F4xx_StdPeriph_Driver 放的是 STM32F4 标准外设固件库源码文件和对应的头文件。
  - inc 目录存放的是 stm32f4xx_ppp.h 头文件,无需改动。
  - src 目录下面放的是 stm32f4xx_ppp.c 格式的固件库源码文件。每一个.c 文件和一个相应的.h 文件对应。这里的文件也是固件库外设的关键文件，每个外设对应一组文件。

Project 文件夹下面有两个文件夹。顾名思义， STM32F4xx_StdPeriph_Examples 文件夹下面存放的的 ST 官方提供的固件实例源码，在以后的开发过程中，可以参考修改这个官方提供的实例来快速驱动自己的外设，很多开发板的实例都参考了官方提供的例程源码，这些源码对以后的学习非常重要。 STM32F4xx_StdPeriph_Template 文件夹下面存放的是工程模板。

Utilities 文件下就是 __官方评估板__ 的一些对应源码，

根目录中还有一个 stm32f4xx_dsp_stdperiph_lib_um.chm 文件，直接打开可以知道，这是一个固件库的帮助文档，这个文档非常有用

几个重要文件:

1. core_cm4.h 文件位于\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\CMSIS\Include 目录下面的，这个就是 CMSIS 核心文件，提供进入 M4 内核接口，这是 ARM 公司提供，对所有CM4 内核的芯片都一样。你永远都不需要修改这个文件.

2. stm32f4xx.h 和 system_stm32f4xx.h 文件存放在文件夹\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include 下面。system_stm32f4xx.h 是片上外设接入层系统头文件。主要是申明设置系统及总线时钟相关的函数。与其对应的源文件 system_stm32f4xx.c 在目录\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Project\STM32F4xx_StdPeriph_Templates 可以找到。 这个里面有一个非常重要的 SystemInit()函数申明，这个函数在我们系统启动的时候都会调用，用来设置系统的整个系统和总线时钟。

3. stm32f4xx.h 是 STM32F4 片上外设访问层头文件。 这个文件就相当重要了，只要你做STM32F4 开发，你几乎时刻都要查看这个文件相关的定义。这个文件打开可以看到，里面非常多的结构体以及宏定义。 这个文件里面主要是系统寄存器定义申明以及包装内存操作

4. stm32f4xx_it.c,stm32f4xx_it.h 以 及 stm32f4xx_conf.h 等 文 件 ， 我 们 可 以 从\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Project\STM32F4xx_StdPeriph_Templates 文件夹中找到。这几个文件我们后面新建工程也有用到。 stm32f4xx_it.c 和 stm32f4xx_it.h 里面是用来编写中断服务函数，中断服务函数也可以随意编写在工程里面的任意一个文件里面，个人觉得这个文件没太大意义。stm32f4xx_conf.h 是外设驱动配置文件。 文件打开可以看到一堆的#include,这里你建立工程的时候，可以注释掉一些你不用的外设头文件。

5. misc.c,misc.h,stm32f4xx_ppp.c,stm32f4xx_ppp.h 以及 stm32f4xx_rcc.c 和 stm32f4xx_rcc.h 文件，这些文件存放在目录 Libraries\STM32F4xx_StdPeriph_Driver。这些文件是 STM32F4 标准的外设库文件。其中 misc.c 和 misc.h 是定义中断优先级分组以及 Systick 定时器相关的函数。 stm32f3xx_rcc.c 和 stm32f4xx_rcc.h 是与 RCC 相关的一些操作函数，作用主要是一些时钟的配置和使能。在任何一个 STM32 工程 RCC 相关的源文件和头文件是必须添加的。对于文件 stm32f4xx_ppp.c 和 stm32f4xx_ppp.h，这就是 stm32F4 标准外设固件库对应的源文件和头文件。包括一些常用外设 GPIO,ADC,USART 等。

6. 文件 Application.c 实际就是说是应用层代码。这个文件名称可以任意取了。 我们工程中，直接取名为 main.c。

7. 实际上一个完整的 STM32F4 的工程光有上面这些文件还是不够的。还缺少非常关键的启动文件。 STM32F4 的启动文件存放在目录\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries
\CMSIS\Device\ST\STM32F4xx\Source\Templates\arm 下面。对于不同型号的 STM32F4 系列对应
的启动文件也不一样。我们的开发板是 STM32F407 系列所以我们选择的启动文件为
startup_stm32f40_41xxx.s。 




####Handler机制
系统main函数是单线程的, 子线程不能修改主线程的任何操作. 所以要用一种方式让子线程中的信息返回到主线程中再做修改. 这就要让主线程与子线程通信.  如何让他们之间进行通信, 用handler这个类, handler操作的是message. 在主线程中, 消息队列是在程序启动时就自动生成的, 而子线程要自己手动建立消息队列.    
Handler是在主线程中,  它的触发是通过SendMessage()这个方法驱动的, HandleMessage()和SendMessage()是一对, 一个负责接收一个负责发送. 

子线程请求回来的数据, 为了把它交给主线程, 就要在这之前在主线程中创建handler, 然后实现 handleMessage 方法. 
handler机制被引入的目的就是为了实现线程间通信的。handler一共干了两件事：在子线程中发出message，在主线程中获取、处理message。  
 为了能让主线程“适时”得处理子线程所发送的message，显然只能通过回调的方式来实现——开发者只要重写Handler类中处理消息的方法，当子线程发送消时，Handler类中处理消息的方法就会被自动回调。﻿  
子线程通过主线程的handler将消息放到主线程的消息队列. 主进程从消息队列里取出消息再调用Handler的handlemessage来实现主线程中的各种方法. 


####基础知识
1. `extern`变量声明:  
C 语言中 extern 可以置于变量或者函数前，以表示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。

2. typedef使用:
如果不使用typedef, 类似的结构体比如:
```C
struct GPIO_{
  __IO uint32_t MODER;
  __IO uint32_t OTYPER;
  //...
};  //这样就定义了一个结构体

struct GPIO_ GPIOA;
```
使用了typedef之后:
```C
typedef struct{
  __IO uint32_t MODER;
  __IO uint32_t OTYPER;
  //...
  }GPIO_Type;

GPIO_Type GPIOA,GPIOB;
```
这里的 GPIO_Type 就跟 struct _GPIO 是等同的作用了. 

3. struct 结构体:  

```C
Struct U_TYPE {
Int BaudRate
Int WordLength;
}usart1,usart2;

//在结构体申明的时候可以定义变量，也可以申明之后定义，

struct U_TYPE usart1,usart2;
```

结构体成员变量的引用方法是： `结构体变量名字.成员名`
比如要引用 usart1 的成员 BaudRate，方法是： `usart1.BaudRate;`  
结构体指针变量定义也是一样的，跟其他变量没有啥区别。
例如： struct U_TYPE *usart3； //定义结构体指针变量 usart1;
结构体指针成员变量引用方法是通过“ ->”符号实现，比如要访问 usart3 结构体指针指向的结
构体的成员变量 BaudRate,方法是：
Usart3->BaudRate;



4. 为什么要使用结构体?  
在我们单片机程序开发过程中，经常会遇到要初始化一个外设比如串口，它的初始化状态是由几个属性来决定的，比如串口号，波特率，极性，以及模式等。对于这种情况，在我们没有学习结构体的时候，我们一般的方法是：  
`void USART_Init(u8 usartx,u32 u32 BaudRate,u8 parity,u8 mode);`  
这种方式是有效的同时在一定场合是可取的。但是试想，如果有一天，我们希望往这个函数里面再传入一个参数，那么势必我们需要修改这个函数的定义，重新加入字长这个入口参数。于
是我们的定义被修改为：  
`void USART_Init (u8 usartx,u32 BaudRate, u8 parity,u8 mode,u8 wordlength );`   
但是如果我们这个函数的入口参数是随着开发不断的增多，那么是不是我们就要不断的修改函数的定义呢？这是不是给我们开发带来很多的麻烦？那又怎样解决这种情况呢？这样如果我们使用到结构体就能解决这个问题了。我们可以在不改变入口参数的情况下，只需要改变结构体的成员变量，就可以达到上面改变入口参数的目的。  
结构体就是将多个变量组合为一个有机的整体。上面的函数， BaudRate,wordlength,Parity,mode,wordlength 这些参数，他们对于串口而言，是一个有机整体，都是来设置串口参数的，所以我们可以将他们通过定义一个结构体来组合在一个。  
比如:
```C
typedef struct
{
uint32_t USART_BaudRate;
uint16_t USART_WordLength;
uint16_t USART_StopBits;
uint16_t USART_Parity;
uint16_t USART_Mode;
uint16_t USART_HardwareFlowControl;
} USART_InitTypeDef;

//于是，我们在初始化串口的时候入口参数就可以是 USART_InitTypeDef 类型的变量或者指针变量了

void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
```
这样，任何时候，我们只需要修改结构体成员变量，往结构体中间加入新的成员变量，而不需要修改函数定义就可以达到修改入口参数同样的目的了。 这样的好处是不用修改任何函数定义就可以达到增加变量的目的  

5. 时钟
为什么 STM32 要有多个时钟源呢？ 因为首先 STM32 本身非常复杂，外设非常的多，但是并不是所有外设都需要系统时钟这么高的频率，比如看门狗以及 RTC 只需要几十 k 的时钟即可。同一个电路，时钟越快功耗越大，同时抗电磁干扰能力也会越弱，所以对于较为复杂的 MCU 一般都是采取多时钟源的方法来解决这些问题。

总结一下 SystemInit()函数中设置的系统时钟大小：    
SYSCLK（系统时钟） =168MHz    
AHB 总线时钟(HCLK=SYSCLK) =168MHz   
APB1 总线时钟(PCLK1=SYSCLK/4) =42MHz  
APB2 总线时钟(PCLK2=SYSCLK/2) =84MHz  
PLL 主时钟 =168MHz   

时钟配置函数:
一类是外设时钟使能函数，一类是时钟源和分频因子配置函数，还有一类是外设复位函数。当然还有几个获取时钟源配置的函数。  
时钟使能函数:  
`void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);`   

STM32F4 的外设在使用之前，必须对时钟进行使能，如果没有使能时钟，那么外设是无法正常工作的。对于哪个外设是挂载在哪个总线之下，虽然也可以查手册查询到，但是这里如果大家使用的是库函数的话，实际上是没有必要去查询手册的. 例如:  
如果我们要使能GPIOA, 只需要在stm32f4xx_rcc.h头文件里搜索GPIOA, 就可以搜索到对应的时钟使能函数的第一个入口参数为`RCC_AHB1Periph_GPIOA`, 从这个宏定义标识符可以看出, GPIOA是挂载在AHB1下边的.同理，对于串口 1 我们可以搜索 USART1，找到标识符为 `RCC_APB2Periph_USART1`，那么很容易知道串口 1 是挂载在 APB2 之下。  
知道了之后, 调用AHB1总线下外设时钟使能函数`RCC_AHB1PeriphClockCmd`:  
`RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);`  

还有一类时钟使能函数是时钟源使能函数, STM32F4有5个大类时钟源:  

```C
void RCC_HSICmd(FunctionalState NewState);
void RCC_LSICmd(FunctionalState NewState);
void RCC_PLLCmd(FunctionalState NewState);
void RCC_PLLI2SCmd(FunctionalState NewState);
void RCC_PLLSAICmd(FunctionalState NewState);
void RCC_RTCCLKCmd(FunctionalState NewState);

//具体调用方法:
RCC_PLLCmd(ENABLE);
```

第二类时钟功能函数：时钟源选择和分频因子配置函数。这些函数是用来选择相应的时钟源以及配置相应的时钟分频系数。 例如SYSCLK, 可以选择HSI,HSE以及PLL三个中的一个时钟源为系统时钟.  例如要设置系统时钟源为HSI, 那么可以调用系统时钟源配置函数:   
`RCC_HCNKConfig(RCC_SYSCLKSource_HSI);  //配置时钟源为HSI`  
或者要设置APB1总线时钟为HCLK的2分频, 也就是设置分频因子为2分频, 具体设置方法为:     
`RCC_PCLK1Config(RCC_HCLK_Div2);`  
(APB1, APB2分别对应PCLK1, PCLK2. APB1低速外设时钟, APB2为高速.)

第三类外设复位函数:
```C
void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
```

#### 定时器中断
定时器配置步骤:
1. TIM3时钟使能: TIM3是挂载在APB1下, 所以我们通过APB1总线下的使能函数来使能TIM3.  调用的函数是:   
`RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM3时钟`  
2. 初始化定时器参数, 设置自动重装值, 分频系数, 计数方式: 再库函数中, 定时器的初始化参数是通过初始化函数`TIM_TimeBaseInit`实现的.  
`void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);`  
第一个参数是确定是哪个定时器, 第二个参数是定时器初始化参数结构体指针, 结构体类型为`TIM_TimeBaseInitTypeDef`. 该结构体定义为:  

```C
typedef struct
{
  uint16_t TIM_Prescaler;
  uint16_t TIM_CounterMode;
  uint16_t TIM_Period;
  uint16_t TIM_ClockDivision;
  uint8_t TIM_RepetitionCounter;
} TIM_TimeBaseInitTypeDef;
```

第一个参数 TIM_Prescaler 是用来设置分频系数的; 第二个参数 TIM_CounterMode 是用来设置计数方式，可以设置为向上计数，向下计数方式还有中央对齐计数方式，比较常用的是向上计数模式 TIM_CounterMode_Up 和向下计数模式 TIM_CounterMode_Down; 第三个参数是设置自动重载计数周期值; 第四个参数是用来设置时钟分频因子。 最后一个参数 TIM_RepetitionCounter 是高级定时器才有用的

针对 TIM3 初始化范例代码格式：
```C
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_TimeBaseStructure.TIM_Period = 5000;
TIM_TimeBaseStructure.TIM_Prescaler =7199;
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
```
3. 设置TIM3_DIER允许更新中断: 因为我们要使用TIM3的更新中断, 寄存器相应位就可使能更新中断. 库函数里定时器中断使能是通过TIM_ITConfig函数来实现的.  
`void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState); `  
第一个参数是选择定时器号, 取值为TIM1~TIM17; 第二个参数用来指明使能的定时器中断的类型, 包括更新中断TIM_IT_Update, 触发中断TIM_IT_Trigger, 以及捕获中断等等; 第三个参数就是使能或者失能. 例如我们使能TIM3更新中断:  
`TIM_ITConfig(TIM3, TIM_IT_UPDATE, ENABLE);`
4. TIM3中断优先级设置: 定时器中断使能之后, 因为要产生中断, 必不可少要设置NVIC相关寄存器, 设置中断优先级. 
5. 允许TIM3工作, 也就是使能TIM3: 配置完后要开启定时器, 通过TIM3_CR1的CEN位来设置, 在固件库里面使能定时器的函数是通过TIM_Cmd函数来实现的.  
`void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);`
6. 编写中断服务函数: 通过该函数来处理定时器产生的相关中断。在
中断产生后，通过状态寄存器的值来判断此次产生的中断属于什么类型。然后执行相关的操作，这里使用的是更新（溢出）中断，所以在状态寄存器 SR 的最低位。在处理完中断之后应该向 TIM3_SR 的最低位写 0，来清除该中断标志。固件库中, 用来读取中断状态寄存器的值判断中断类型的函数是:   
`ITStatus TIM_GetITStatus(TIM_TypeDef *TIMx, uint16_t);`  
作用是来判断定时器TIMx的中断类型TIM_IT是否发生中断. 比如要判断定时器3是否发生更新中断, 方法是:  
`if( TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET ) {}`  
清除中断标志位的函数:   
`void TIM_ClearITPendingBit(TIM_TypeDef *TIMx, uint16_t TIM_IT)`  
比如在TIM3的溢出中断发生后, 要清除中断标志位, 方法是  
`TIM_ClearITPendingBit(TIM3, TIM_IT_Update);`  

这里需要说明一下，固件库还提供了两个函数用来判断定时器状态以及清除定时器状态标志位的函数 TIM_GetFlagStatus 和 TIM_ClearFlag，他们的作用和前面两个函数的作用类似。只是在 `TIM_GetITStatus `函数中会先判断这种中断是否使能，使能了才去判断中断标志位，而TIM_GetFlagStatus 直接用来判断状态标志位。

因此初始化函数  
`void TIM3_Int_Init(u16 arr,u16 psc);`  
中的2个参数用来设置TIM3的溢出时间. 因为系统初始化 SystemInit 函数里面已经初始化 APB1 的时钟为 4 分频，所以 APB1 的时钟为 42M， 而从 STM32F4 的内部时钟树图得知：当 APB1 的时钟
分频数为 1 的时候， TIM2~7 以及 TIM12~14 的时钟为 APB1 的时钟，而如果 APB1 的时钟分频数不为 1，那么 TIM2~7 以及 TIM12~14 的时钟频率将为 APB1 时钟的两倍。因此， TIM3 的时钟为 84M，再根据我们设计的 arr 和 psc 的值，就可以计算中断时间了。  psc一般使用`(8400-1)`分频.  
公式:  `Tout= ((arr+1)*(psc+1))/Tclk；`  
其中: 
Tclk: TIM3的输入时钟频率(MHz)  
Tout: TIM3的溢出时间(us)  

#### PWM输出
PWM Pulse Width Modulation 脉冲宽度调制. STM32F4 的定时器除了 TIM6 和 7。其他的定时器都可以用来产生 PWM 输出。其中高级定时器 TIM1 和 TIM8 可以同时产生多达 7 路的 PWM 输出。而通用定时器也能同时产生多达 4路的 PWM 输出(CH1, CH2, CH3, CH4).   
要使STM32F4的通用定时器TIMx产生PWM输出, 除了普通定时要用到的寄存器外, 还会用到3个寄存器来控制PWM. 分别是: `捕获/比较模式寄存器(TIMx_CCMR1/2)`, `捕获/比较使能寄存器(TIMx_CCER)`, `捕获/比较寄存器(TIMx_CCR1~4)`.

1. TIMx_CCMR1/2: Capture/Compare Mode Register该寄存器一般有2个 (TIM14只有1个): CCMR1和CCMR2. TIMx_CCMR1控制CH1, CH2, TIMx_CCMR2控制CH3, CH4.   
关于TIM14_CCMR1: 该寄存器的有些位在不同模式下(比如输入输出), 功能是不一样的.   
(1) OC1M模式设置位(3 Bit). 总共可以配置成7种模式, 要设置成PWM, 这3位必须设置为110/111(输出电平的极性相反). 
(2) CC1S通道方向设置(输入输出)默认设置为0(输出). 

2. TIM14_CCER: Capture/Compare Enable Register. 控制着各个输入输出通道的开关. 其中CC1E位是输入/捕获1输出使能位. 要想PWM 从 IO 口输出，这个位必须设置为 1. 

3. TIMx_CCR1~4: 该寄存器共有4个, 对应4个通道CH1~4. 不过TIM14只有1个, 即TIM14_CCR1.   
(1) 如果通道CC1配置为输出: CCR1为要装载到实际捕获/比较1寄存器的值.   
如果没有通过TIMx_CCMR寄存器中的OC1PE位来使能预装载功能, 写入的数值会被直接传输至当前寄存器中.    
(2) 如果通道CC1配置为输入: CCR1为上一个输入捕获1事件(IC1)发生时的计数器值.   
在输出模式下, 该寄存器的值与CNT的值比较, 这样就可以控制PWM的输出脉宽了. 


#### 输入捕获
例子中使用TIM5的通道1(PA0)来做输入捕获. 捕获PA0上高电平的脉宽.输入捕获可以用来测量脉冲宽度或者频率.  
工作原理: 出现电平变化, 开始计数(设定一个溢出值, 达到溢出值, 清零重新计算, 假设溢出了N次, 溢出值为ARR), 直到再次捕获到电平变化, 停止计数, 此时计数器值为CRRx.则计数次数为`N*ARR+CRRx`. 再乘以计数周期即为脉冲时间. 

#### GNU C 和 ANSI C
Linux上使用的C编译器是GNU C编译器，其对标准的C（ansi c）进行了一定的扩展，这带来的影响是两方面的。一方面增强了其原来的没有的功能，另一方面却对要编写移植性要求较高的程序带来了一些问题。对于后一个问题，在编写程序时，建议是如果在ANSI C中也提供的同样的功能时，尽量使用ANSI C来实现.   
1. 变量长度数组  
在标准C中，我们都知道实例化一个数组的时候，其大小必须是常量。GNU C可以使用一个变量的数值来实例化一个数组。例如，下面的程序是正确的。
```C
      int i=10;
	  char aa[i];
```
2. 零长度数组  
GNU C中是可以的, 但是并不表示这个数组的长度为0, 这只是一种表现形式. 例如:   
```C
	struct var_d{
	int len;
	char data[0];
};
```
这种表现形式一般用在可变长度的BUFF中, 会发现 `sizeof(struct var_d) == sizeof(int)` 这说明data不占任何空间的. 其实data就是一个常量指针, 指向用上述结构体实例化对象所占内存空间的下一个地址. 用法一般为:  
```C
struct var_d *thisline;
thisline = (struct var_d *)malloc(sizeof(struct var_d) + 10);
thisline -> len = 10;
```
此时data指向的空地址就是上述后面多分配出来的10个字节的首地址, 对于可变的buff, 完全可以用data[i]来读出数据. 

[Reference](http://blog.csdn.net/ssdsafsdsd/article/details/8234736)  

3. 标号元素
 标准C要求数组或结构体的初始化数值必须以固定的顺序出现.GNU C，可以通过指定索引或结构体的成员名来以任意的顺序进行初始化。  
 (1) 数组初始化:  数组的初始化是通过指定数组的索引来实现，在初始化一个数值前在前面添加索引`[INDEX]=`,也可以使用`[FIRST...LAST]=`来指定一个范围。
 (2) 结构体初始化: 用结构体的成员名来指定.   

 例子如下:  

```C
 //数组初始化例子:
 #define LEN 10

unsigned char da[LEN] = {[3] = 'a'}; 	//通过这个方式将da[3]初始化为'a'
unsigned char da[LEN] = {[0...LEN-1] = 0}; 	//将数组da全部初始化为0

//结构体初始化例子: 
struct date{
	int year;
	int month;
	int day;
	char hour;
	char min;
	char sec;
};
typedef struct date DATE;

//GNU C实现初始化
DATE dd{
	year:2020,
	month: 2,
	day: 27,
	hour: 16,
	min: 43,
	sec: 21,

};

//ANSI C实现初始化
DATE CC{
	.year = 2002,
	.month = 2,
	.day = 22,
};

```

4. 特殊属性声明
GNU C允许声明函数、变量和类型的特殊属性，以便进行手工的代码优化和定制代码检查方法。要指定一个声明的属性，主需要在声明后添加__attribute__((ATTRIBUTE))。其中ATTRIBUTE为属性声明，如果存在多个属性，则以逗号分开。GNU C支持noreturn、format、section、aligned、packed等十多个属性。  
`noreturn` 属性用于函数, 表示该函数从不返回. 这会让编译器优化代码，并消除不必要的警告信息。  
`format` 属性用于函数, 表示函数使用printf,scanf和strftime风格的参数指定format属性可以让编译器根据格式串检查参数类型。  
`unused` 用于变量或函数，当他们未被用到时，不会提示警告信息。  


#### 字节对齐
1. 对齐跟数据在内存中的位置有关。如果一个变量的内存地址正好位于它长度的整数倍，他就被称做自然对齐。比如在32位cpu下，假设一个整型变量的地址为0x00000004，那它就是自然对齐的。  
需要字节对齐的根本原因在于CPU访问数据的效率问题。假设上面整型变量的地址不是自然对齐，比如为0x00000002，则CPU如果取它的值的话需要访问两次内存，第一次取从0x00000002-0x00000003的一个short，第二次取从0x00000004-0x00000005的一个short然后组合得到所要的数据，如果变量在0x00000003地址上的话则要访问三次内存，第一次为char，第二次为short，第三次为char，然后组合得到整型数据。  
2. 对于标准数据类型，它的地址只要是它的长度的整数倍就行了，而非标准数据类型按下面的原则对齐：  
数组：按照基本数据类型对齐，第一个对齐了后面的自然也就对齐了。   
联合 ：按其包含的长度最大的数据类型对齐。   
结构体： 结构体中每个数据类型都要对齐。  
For Example:  
```C
struct stu{
　　 char sex;
　　 int length;
　　 char name[10];
　　};
　　struct stu my_stu;

```
由于在x86下，GCC默认按4字节对齐，它会在sex后面跟name后面分别填充三个和两个字节使length和整个结构体对齐。于是我们sizeof(my_stu)会得到长度为20，而不是15.

3. 我们可以按照自己设定的对齐大小来编译程序，GNU使用__attribute__选项来设置，比如我们想让刚才的结构按一字节对齐，我们可以这样定义结构体:   
```C
struct stu{
　　 char sex;
　　 int length;
　　 char name[10];
　　}__attribute__ ((aligned (1))); 
　　
　　struct stu my_stu;
```
则sizeof(my_stu)可以得到大小为15。

4. 在设计不同CPU下的通信协议时，或者编写硬件驱动程序时寄存器的结构这两个地方都需要按一字节对齐。即使看起来本来就自然对齐的也要使其对齐，以免不同的编译器生成的代码不一样.

5. 在缺省情况下，C编译器为每一个变量或是数据单元按其自然对界条件分配空间。一般地，可以通过下面的方法来改变缺省的对界条件：
- 使用伪指令#pragma pack (n)，C编译器将按照n个字节对齐。
- 使用伪指令#pragma pack ()，取消自定义字节对齐方式。
另外，还有如下的一种方式：
- __attribute((aligned (n)))，让所作用的结构成员对齐在n字节自然边界上。如果结构中有成员的长度大于n，则按照最大成员的长度来对齐。
- __attribute__ ((packed))，取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐。

[Reference](http://blog.csdn.net/21aspnet/article/details/6729724) 





#### __attribute__ ((packed)) 
1. __attribute__ ((packed))的作用就是告诉编译器_取消_结构在编译过程中的优化对齐. 按照实际占用字节数进行对齐, 是_GCC_特有的语法. 这个功能跟编译器有关. GCC编译器默认的不是紧凑模式.
```C
//在GCC下：
struct my{ char ch; int a;} sizeof(int)=4;sizeof(my)=8;	//（默认: 非紧凑模式）

//在GCC下：
struct my
{ 	char ch; 
	int a;
}__attrubte__ ((packed));

//sizeof(int)=4;sizeof(my)=5
```

`__attribute__`书写特征是：`__attribute__`前后都有两个下划线，并且后面会紧跟一对括弧，括弧里面是相应的`__attribute__`参数。  
__attribute__语法格式为：  

`__attribute__ ((attribute-list))`  

其位置约束：放于声明的尾部“；”之前。  

2. packed属性：使用该属性可以使得变量或者结构体成员使用最小的对齐方式，即对变量是一字节对齐，对域（field）是位对齐。  
packed是类型属性（Type Attribute）的一个参数，使用packed可以减小对象占用的空间。需要注意的是，attribute属性的效力与你的连接器也有关，如果你的连接器最大只支持16字节对齐，那么你此时定义32字节对齐也是无济于事的。


#### OLED 
Organic Light Emitting Diode 有机发光二极管. 自发光, 不需背光源, 对比度高, 反应速度快. 开发板OLED模块控制器为`SSD1306`.该模块提供4种连接方式: `6800`,`8080`两种并行接口方式, 4线`SPI`以及`IIC`接口方式.  
`8080`并行接口: 该总线被广泛应用于各种液晶显示器. 8080接口方式需要以下信号线:   
* CS: 片选信号线
* WR: 向OLED写入数据
* RD: 从OLED读取数据
* D[7:0]: 8位双向数据线
* RST(RES): 硬件复位
* DC: 数据命令标志(1:读写数据, 0: 读写命令)

模块的 8080 并口读/写的过程为:先根据要写入/读取的数据的类型,设置 DC 为高(数据) /低(命令),然后拉低片选,选中 SSD1306,接着我们根据是读数据,还是要写数据置 RD/WR 为低,然后:
在 RD 的上升沿, 使数据锁存到数据线(D[7:0])上;在 WR 的上升沿,使数据写入到 SSD1306 里面. 

在 8080 方式下读数据操作的时候,我们有时候(例如读显存的时候)需要一个假读命
(Dummy Read),以使得微控制器的操作频率和显存的操作频率相匹配。在读取真正的数据之 前,由一个的假读的过程。这里的假读,其实就是第一个读到的字节丢弃不要,从第二个开始, 才是我们真正要读的数据。  
 
SPI通信模式: 所用信号线:  
* CS: 片选信号线
* RST(RES): 硬件复位
* DC: 命令/数据 标志 (0: 命令, 1: 数据)
* SCLK: 串行时钟线
* SDIN: 串行数据线  
注: 4线串行模式下, 只能往模块写数据而不能读数据  
在 4 线 SPI 模式下,每个数据长度均为 8 位,在 SCLK 的上升沿,数据从 SDIN 移入到
SSD1306,并且是高位在前的。DC 线还是用作命令/数据的标志线。

SSD1306显存: 总共128*64bit, 被分成8页, 每页包含128个字节. 在STM32F4 的内部建立一个 OLED 的 GRAM(共 128*8 个字节), 在每次修改的时候,只是修改 STM32F4 上的 GRAM(实际上就是 SRAM),在修改完了之后, 一次性把 STM32F4 上的 GRAM 写入到 OLED 的 GRAM。当然这个方法也有坏处,就是对于 那些 SRAM 很小的单片机(比如 51 系列)就比较麻烦了。  
GRAM(Graphic RAM)图像寄存器
 

#### TFTLCD
通过STM32的FSMC接口来控制TFTLCD的显示. TFTLCD即薄膜晶体管液晶显示器(Thin Film Transistor - Liquid Crystal Display). 该模块接口为16位的80接口, 自带触摸屏, 可以用来作为控制输入.   
该模块80接口信号线: 
* CS: TFTLCD片选信号线
* WR: 写数据信号线
* RD: 读数据信号线
* D[15:0]: 16位双向数据线
* RST: 硬件复位
* RS: 命令/数据标志(0: 命令, 1: 数据)
* 另外我们还需要一个背光控制线来控制 TFTLCD 的背光

驱动芯片以ILI9341为例. ILI9341液晶控制器自带显存,  为`240*320*18*8 = 172800`, 即18位模式(26万色)下的显存量. 在 16 位模式下,ILI9341 采用 RGB565 格式存储颜色数据. 这样 MCU 的 16 位数据,最低 5 位代表蓝色,中间 6 位为绿色,最高 5 位为红色。数值越 大,表示该颜色越深。另外,特别注意 ILI9341 所有的指令都是 8 位的(高 8 位无效),且参数 除了读写 GRAM 的时候是 16 位,其他操作参数,都是 8 位的. 


###### ILI9341的命令: 
0XD3,0X36,0X2A,0X2B,0X2C,0X2E 等 6 条指令.  
1. 0XD3: 用于读取 LCD 控制器的 ID
2. 0X36: 存储访问控制指令,可以控制 ILI9341 存储器的读写方向,简 单的说,就是在连续写 GRAM 的时候,可以控制 GRAM 指针的增长方向,从而控制显示方式
(读 GRAM 也是一样). 指令后边跟一个参数, 该参数主要关注三位: MY, MX, MV. 通过这三位来控制整个ILI9341的扫描方向. 
3. 0X2A: 列地址设置指令,在从左到右,从上到下的扫描方式(默认)下面,该指令用于设置横坐标. 在默认扫描方式时,该指令用于设置 x 坐标,该指令带有 4 个参数,实际上是 2 个坐标值:
SC 和 EC,即列地址的起始值和结束值,SC 必须小于等于 EC,且 0≤SC/EC≤239。 
4. 0X2B: 是页地址设置指令,在从左到右,从上到下的扫描方式 (默认)下面,该指令用于设置纵坐标(y 坐标). 在默认扫描方式时,该指令用于设置 y 坐标,该指令带有 4 个参数,实际上是 2 个坐标值:
SP 和 EP,即页地址的起始值和结束值,SP 必须小于等于 EP,且 0≤SP/EP≤319。
5. 0X2C: 该指令是写 GRAM 指令,在发送该指令之后,我们便可以往 LCD 的 GRAM 里面写入颜色数据了,该指令支持连续写. 在收到指令 0X2C 之后,数据有效位宽变为 16 位,我们可以连续写入 LCD GRAM 值,而 GRAM 的地址将根据 MY/MX/MV 设置的扫描方向进行自增。
6. 0X2E: 该指令是读 GRAM 指令,用于读取 ILI9341 的显存(GRAM). 该指令用于读取 GRAM, ILI9341 在收到该指令后,第一次输出的是
dummy 数据,也就是无效的数据,第二次开始,读取到的才是有效的 GRAM 数据(从坐标: SC,SP 开始),输出规律为:每个颜色分量占 8 个位,一次输出 2 个颜色分量。比如:第一次 输出是 R1G1,随后的规律为:B1R2 -> G2B2 -> R3G3 -> B3R4 -> G4B4 -> R5G5... 以此类推。

###### TFTLCD使用流程
硬复位 (LCD_RST = 0; delay_ms(100); LCD_RST = 1;)-> 初始化序列 -> 设置坐标   
.. (读) -> 读GRAM指令 -> 读出颜色数据 -> 单片机处理   
.. (写) -> 写GRAM指令 -> 写颜色数据 -> LCD显示

###### FSMC
要使用LCD, 首先将与 TFTLCD 模块相连的 IO 口进行初始化,以便驱动 LCD。这里我们用 到的是FSMC. FSMC(Flexible Static Memory Controller), 灵活的静态存储控制器. 能够与同步或异步存储器和16位PC存储器卡链接. STM32F4的FSMC接口支持包括 SRAM, NAND FLASH, NOR FLASH等.   
STM32F4的FSMC分为2类: NOR/PSRAM 设备, NAND/PC卡设备. 共用地址数据总线等信号, 不同的CS(片选信号线)来区分不同的设备.   
这里用到的TFTLCD就是用FSMC_NE4做片选, 其实就是将TFTLCD当做SRAM来控制.  
原因:  外部 SRAM 的控制一般有:地址线(如 A0~A18)、数据线(如 D0~D15)、写信号(WE)、 读信号(OE)、片选信号(CS),如果 SRAM 支持字节控制,那么还有 UB/LB 信号. TFTLCD控制线包括: RS、D0~D15、WR、RD、CS、RST 和 BL 等,其中真正在操作 LCD的时候需要用到的就只有:RS、D0~D15、WR、RD 和 CS。其操作时序和 SRAM的控制完全类似,唯一不同就是 TFTLCD 有 RS 信号,但是没有地址信号。
TFTLCD 通过 RS 信号来决定传送的数据是数据还是命令,本质上可以理解为一个地址信 号,比如我们把 RS 接在 A0 上面,那么当 FSMC 控制器写地址 0 的时候,会使得 A0 变为 0, 对 TFTLCD 来说,就是写命令。而 FSMC 写地址 1 的时候,A0 将会变为 1,对 TFTLCD来说,就是写数据了。这样,就把数据和命令区分开了,他们其实就是对应 SRAM 操作的两个连续地
址。  
STM32F4 的 FSMC 支持 8/16/32 位数据宽度,这里用到的 LCD 是 16 位宽度的,所以在设置的时候,选择 16 位宽就 OK 了。再来看看 FSMC的外部设备地址映像,STM32F4的 FSMC 将外部存储器划分为固定大小为 256M 字节的四个存储块. FSMC 总共管理 1GB 空间,拥有4个存储块(Bank),这里我们用到的是块 1(Bank 1).  
STM32F4 的 FSMC 存储块 1(Bank1)被分为 4 个区,每个区管理 64M 字节空间,每个区 都有独立的寄存器对所连接的存储器进行配置。Bank1 的 256M 字节空间由 28 根地址线 (HADDR[27:0])寻址。 HADDR 是内部 AHB 地址总线,其中 HADDR[25:0]来自外部存储器地址 FSMC_A[25:0],而 HADDR[26:27]对 4 个区进行寻址。HADDR是需要转换到外部存储器的内部AHB地址线.     

块1存储区被划分为4个NOR/PSRAM区，这四个区在内部地址上是连续排列的。但是实际上每个区共用的是同一组地址线与数据线，因此需要有内外的一个地址映射，因此在STM32中实际上有两个地址，一个是在内部访问的地址，另外一个是实际地址线输出的地址。HADDR[27:0]对应的是需要转换到外部存储器的内部AHB地址线，其中HADDR[27:26]位用于选择四个存储块之一。HADDR[25:0]包含外部存储器地址。HADDR是字节地址，而不同的外部存储器数据长度也不一样，因此在数据宽度为8位和16位时映射关系也不一样。在数据宽度为8位时HADDR[25:0]与FSMC_A[25:0]对应相连，这时候在STM32中访问的地址和实际地址线产生的地址是一致的。而在16位数据宽度时HADDR[25:1]与FSMC_A[24:0]对应相连，HADDR[0]未接，这时候实际地址线上给出的地址为需要访问的偏移地址的一半。

FSMC 的 NOR FLASH 控制器支持同步 和异步突发两种访问方式。选用同步突发访问方式时,FSMC 将HCLK(系统时钟)分频后,发 送给外部存储器作为同步时钟信号FSMC_CLK。此时需要的设置的时间参数有 2 个:  
1. HCLK 与 FSMC_CLK 的分频系数(CLKDIV),可以为 2~16 分频;  
2. 同步突发访问中获得第 1 个数据所需要的等待延迟(DATLAT)。  
对于异步突发访问方式,FSMC 主要设置 3 个时间参数:地址建立时间(ADDSET)、数据建立时间(DATAST)和地址保持时间(ADDHLD)。FSMC 综合了 SRAM/ROM、PSRAM 和 NOR Flash 产品的信号特点,定义了 4 种不同的异步时序模型。   在实际扩展时,根据选用存储器的特征确定时序模型,从而确定各时间参数与存储器读/写周期参数指标之间的计算关系;利用该计算关系和存储芯片数据手册中给定的参数指标,可计算出 FSMC所需要的各时间参数,从而对时间参数寄存器进行合理的配置。  
这里使用异步模式A(Mode A), 模式A需要配置的参数就是地址建立时间(ADDSET)、数据建立时间(DATAST). 模式 A 支持独立的读写时序控制,这个对我们驱动 TFTLCD来说非常有用,因为 TFTLCD在读的时候,一般比较慢,而在写的时候可以比较快,如果读写用一样的时序,那么只能以读的时序为基准,从而导致写的速度变慢,或者在读数据的时候,重新配置 FSMC的延时,在读操作完成的时候,再配置回写的时序,这样虽然也不会降低写的速度,但是频繁配置,比较麻烦。而如果有独立的读写时序控制,那么只要初始化的时候配置好,之后就不用再配置,既可以满足速度要求,又不需要频繁改配置。

###### 寄存器配置
ADDSET 与 DATAST,是通过不同的寄存器设置的, 所以关于Bank1的几个控制寄存器:   
1. FSMC_BCRx(x=1~4): SRAM/NOR 闪存片选控制寄存器. 
    * EXTMOD:扩展模式使能位,也就是是否允许读写不同的时序,很明显,我们需要读写不同的时序,故该位需要设置为 1。
    * WREN:写使能位。要向TFTLCD写数据, 此位置1. 
    * MWID[1:0]:存储器数据总线宽度。00,表示 8 位数据模式;01 表示 16 位数据模式;10 和 11 保留。我们使用的 TFTLCD 是 16 位数据线,所以设置 WMID[1:0]=01。
    * MTYP[1:0]:存储器类型。00 表示 SRAM、ROM;01 表示 PSRAM;10 表示 NOR FLASH;11 保留。前面提到,我们把 TFTLCD 当成 SRAM 用,所以需要设置 MTYP[1:0]=00。
    * MBKEN:存储块使能位。
 
2. FSMC_BTRx(x=1~4): SRAM/NOR 闪存片选时序寄存器(读时序寄存器).   这个寄存器包含了每个存储器块的控制信息,可以用于 SRAM、ROM 和 NOR 闪存存储器。
如果 FSMC_BCRx 寄存器中设置了 EXTMOD 位,则有两个时序寄存器分别对应读(本寄存器) 和写操作(FSMC_BWTRx 寄存器)。所以 EXTMOD 是使能了的,也就是本寄存器是读操作时序寄存器,控制读操作的相关时序.   
    * ACCMOD[1:0]:访问模式。00 表示访问模式 A;01 表示访问模式 B;10 表示访问模式 C; 11 表示访问模式 D. 这里用A模式, 所以是00.
    * DATAST[7:0]:数据保持时间。0 为保留设置,其他设置则代表保持时间为: DATAST 个 HCLK 时钟周期,最大为 255 个 HCLK 周期。对 ILI9341 来说,其实就是 RD 低电平持续时间, 一般为 355ns。而一个 HCLK 时钟周期为 6ns 左右(1/168Mhz),为了兼容其他屏,这里设置 DATAST 为60,也就是 60 个 HCLK 周期,时间大约是 360ns。
    * ADDSET[3:0]:地址建立时间。其建立时间为:ADDSET 个 HCLK 周期,最大为 15 个 HCLK 周期。对 ILI9341 来说,这里相当于 RD 高电平持续时间,为 90ns,我们设置 ADDSET 为 15, 即 15*6=90ns. 

3. FSMC_BWTRx(x=1~4): SRAM/NOR 闪存写时序寄存器.   
写操作时序控制寄存器,需要用到的设置同样是:ACCMOD、DATAST和 ADDSET这三个设置。这三个设置的方法同 FSMC_BTRx 一模一样,只是这里对应的是写操作的时序,ACCMOD 设置同 FSMC_BTRx 一模一样,同样是选择模式A,另外 DATAST 和 ADDSET 则对应低电平和高电平持续时间,对 ILI9341来说,这两个时间只需要 15ns 就够了, 比读操作快得多。所以我们这里设置 DATAST 为 2,即 3 个 HCLK 周期,时间约为 18ns。然后 ADDSET 设置为 3,即 3 个 HCLK 周期,时间为 18ns。


注:  
在 MDK 的寄存器定义里面,并 没有定义 FSMC_BCRx、FSMC_BTRx、FSMC_BWTRx等这个单独的寄存器,而是将他们进行 了一些组合。




#### I2C
I2C(Inter-Integrated Circuit) 总线是两线式串行总线, 用于连接微控制器及其外围设备. 由数据线SDA和时钟线SCL构成的, 可发送接收数据. 高速I2C总线一般可达400kbps以上.   
开始信号: SCL 为高电平时， SDA 由高电平向低电平跳变，开始传送数据  
结束信号: SCL 为高电平时， SDA 由低电平向高电平跳变，结束传送数据  
应答信号: 接收数据的 IC 在接收到 8bit 数据后，向发送数据的 IC 发出特定的低电平脉冲，表示已收到数据。 CPU 向受控单元发出一个信号后，等待受控单元发出一个应答信号， CPU 接收到应答信号后，根据实际情况作出是否继续传递信号的判断。若未收到应答信号，由判断为受控单元出现故障。  
I2C分为硬件I2C和软件I2C.硬件比软件模拟快很多, 占用时间少. 硬件I2C只要把数据送到指定的寄存器就可以, 不需要自己写时序. I2C协议主要在乎的是时序的准确性, 所以在没有专门IIC模块的单片机中, 可以用2个口来模拟输出I2C协议的波形. 




#### DMA
DMA: Direct Memory Access. 直接存储器访问. DMA传输方式无需CPU直接控制传输, 也_没有_中断处理方式那样保留现场和恢复现场的过程. 通过硬件为RAM与I/O设备开辟一条直接传送数据的通路. 使CPU效率大为提高.  
STM32F4最多有2个DMA控制器, 共16个数据流(每个控制器8个). 每个DMA控制器都用于管理一个或多个外设的存储器访问请求. 每个数据流可以有最多8个通道. 每个数据流通道都有一个仲裁器, 用于处理DMA请求间的优先级.  (1个DMA控制器 -> 8个数据流 -> 64个通道)   
STM32F4的DMA的一些特性:  
- 双AHB主总线架构, 一个用于存储器访问, 一个用于外设访问. 
- 仅支持32位访问的AHB从编程接口
- 每个DMA控制器有8个数据流, 每个数据流有多达8个通道
- 每个数据流有单独的四级32位先进先出存储器缓冲区(FIFO, First In First Out), 可用于FIFO模式或直接模式.
- 通过硬件可以将每个数据流配置为: 
	1. 支持外设到存储器, 存储器到外设, 存储器到存储器传输的常规通道
	2. 支持在存储器方双缓冲的双缓冲区通道
- 8个数据流中的每一个都连接到专用硬件DMA通道(请求)
- DMA数据流请求之间的优先级可用软件编程(4个级别: 非常高, 高, 中, 低), 在软件优先级相同的情况下可以通过硬件决定优先级(例如请求0的优先级高于请求1).
- 每个数据流也支持通过软件触发 存储器到存储器的传输(限DMA2)
- 可供每个数据流选择的通道请求多达8个. 此选择可由软件配置

存储器到存储器需要外设接口可以访问存储器. 仅DMA2的外设接口可以访问存储器. 所以仅DMA2支持存储器到存储器的传输.   
数据流的多通道选择, 是通过DMA_SxCR寄存器控制的. 

DMA设置相关的寄存器:  
1. DMA中断状态寄存器: `DMA_LISR` 和 `DMA_HISR`. 每个寄存器管理4个数据流, LISR控制0~3, HISR控制4~7.   
`TCIFx` (stream x transfer complete interrupt flag): 数据流x传输完成中断标志. 由硬件置1, 软件清零(通过向DMA_LIFCR对应位写入1).    
`HTIFx` (stream x half transfer interrupt flag): 数据流x半传输中断标志. 硬件置1, 软件清零.
`TEIFx` (stream x transfer error interrupt flag): 数据流x传输错误中断方式. 
硬件置1, 软件清零.
`DMEIFx` (stream x direct mode error interrupt flag): 直接模式错误中断标志.
`FEIFx` (stream x FIFO error interrupt flag): FIFO错误中断标志  

如果开启了DMA_LISR中这些位对应的中断, 则在达到条件后就会跳到中断服务函数中. 如果没有开启, 也可以查询来获得当前DMA传输的状态. 所以我们常用的是`TCIFx`, 此寄存器为只读寄存器. 所以在这些位被硬件置1后, 需要用对应的DMA_LIFCR中的位清零.  

2. DMA中断标志清除寄存器: `DMA_LIFCR` 和 `DMA_HIFCR`. LIFCR负责0~3, HIFCR负责4~7.  
`CTCIFx` (Stream x clear transfer complete interrupt flag): 将此位写1时, DMA_LISR寄存器中相应的`TCIFx`清零.  
`CHTIFx` (clear half transfer interrupt flag), `CTEIFx`, `CDMEIFx`, `CFEIFx`分别对应各自的`DMA_LISR`或`DMA_HISR`中的位. 

3. DMA数据流x数据项数寄存器: `DMA_SxNDTR`. 这个寄存器控制DMA数据流x的每次传输所要传输的数据量. 随传输进行而递减, 这里是数据项数目, 不是字节数, 比如设置数据位宽为16位, 那么传输一次(一个项)就是2个字节.   

4. DMA数据流x的外设地址寄存器: `DMA_SxPAR`. 用来存储STM32F4外设的地址, 比如使用串口1, 该寄存器必须写入 0x40011004 (就是&usart_dr)  

5. DMA数据流x配置寄存器: `DMA_SxCR`. 该寄存器控制着DMA的很多相关信息, 包括数据宽度, 外设及存储宽度, 优先级, 增量模式, 传输方向, 中断允许, 使能. DMA_SxCR是DMA传输的核心控制寄存器.   

6. DMA数据流x的存储器地址寄存器: `DMA_SxM0AR` 和 `DMA_SxM1AR`. 因为STM32F4的DMA支持双缓存. M1AR仅在双缓冲模式下, 才有效. DMA_SxM0AR和DMA_CPARx差不多, 但是是用来放存储器地址的.     
  
  
DMA配置步骤:  
(1) 使能DMA2时钟, 并等待数据流可配置.  
DMA的时钟使能是通过AHB1ENR来控制的, 所以要先使能时钟, 才能配置DMA相关寄存器. 另外, 要对配置寄存器(DMA_SxCR)进行设置, 必须先等待其最低位为0(也就是DMA传输禁止了). 因为  
`#define DMA_SxCR_EN                          ((uint32_t)0x00000001)`  

```C
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);	//DMA2时钟使能

//然后等待DMA可配置, 也就是等待DMA_SxCR寄存器最低位为0
while(DMA_GetCmdStatus(DMA_Streamx) != DISABLE) {}	//等待DMA可配置

```
(2) 初始化DMA2数据流7, 包括配置通道, 外设地址, 存储器地址, 传输数据量  
`void DMA_Init(DMA_Stream_TypeDef *DMAy_Streamx, DMA_Init_TypeDef *DMA_InitStruct);`  
第一个参数是指定初始化的DMA数据流编号(y = 1 or 2; x = 0~7). 第二个参数跟其他外设一样是通过初始化结构体成员变量值来达到初始化的目的. 

```C
typedef struct
{
uint32_t DMA_Channel; 	//用来设置DMA数据流对应的通道	
uint32_t DMA_PeripheralBaseAddr;	// 设置DMA传输的外设基地址
uint32_t DMA_Memory0BaseAddr;	// 内存基地址 存放DMA传输数据的内存地址
uint32_t DMA_DIR;	// 设置数据传输方向, 是从外设读取数据到内存还是从内存读取数据发送到外设
uint32_t DMA_BufferSize;	// 一次传输数据量的大小
uint32_t DMA_PeripheralInc;	//  设置传输数据的时候外设地址是不变还是递增
uint32_t DMA_MemoryInc;		//	设置传输数据时候内存地址是否递增
uint32_t DMA_PeripheralDataSize;	//设置外设的数据长度是为字节传输(8bit), 半字(16bit)还是字(32bit)传输
uint32_t DMA_MemoryDataSize;	//设置内存的数据长度, 类似上一个
uint32_t DMA_Mode;		// 设置DMA模式是否循环采集
uint32_t DMA_Priority;	// 设置DMA通道优先级
uint32_t DMA_FIFOMode;	// 设置是否开启FIFO模式
uint32_t DMA_FIFOThreshold;		// 用来选择FIFO阈值
uint32_t DMA_MemoryBurst;	// 存储器突发传输配置
uint32_t DMA_PeripheralBurst;	//
}DMA_InitTypeDef;
```

第六个参数 DMA_PeripheralInc 设置传输数据的时候外设地址是不变还是递增。如果设置
为递增，那么下一次传输的时候地址加 1，这里因为我们是一直往固定外设地址&USART1->DR发送数据，所以地址不递增，值为 `DMA_PeripheralInc_Disable`. 

第十四个参数 DMA_MemoryBurst 用来配置存储器突发传输配置。可以选择为 4 个节拍的增量突发传输 DMA_MemoryBurst_INC4， 8 个节拍的增量突发传输 DMA_MemoryBurst_INC8， 16 个节拍的增量突发传输 DMA_MemoryBurst_INC16 以及单次传输 DMA_MemoryBurst_Single。

第十五个参数 DMA_PeripheralBurst 用来配置外设突发传输配置。跟前面一个参数
DMA_MemoryBurst 作用类似，只不过一个针对的是存储器，一个是外设。

关于FIFO模式的应用:  
FIFO一般用于不同时钟域之间的数据传输，比如FIFO的一端时AD数据采集，另一端时计算机的PCI总线，假设其AD采集的速率为16位 100K SPS，那么每秒的数据量为100K×16bit=1.6Mbps,而PCI总线的速度为33MHz，总线宽度32bit,其最大传输速率为1056Mbps,在两个不同的时钟域间就可以采用FIFO来作为数据缓冲。

另外对于不同宽度的数据接口也可以用FIFO，例如单片机位8位数据输出，而DSP可能是16位数据输入，在单片机与DSP连接时就可以使用FIFO来达到数据匹配的目的。


(3) 使能串口的DMA发送  
开启串口的DMA发送功能, 即  
`USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);`  

(4) 使能DMA2数据流7, 启动传输  
`void DMA_Cmd(DMA_Stream_TypeDef * DMAy_Streamx, FunctionalState NewState);`  
使能DMA2_Stream7方法: `DMA_Cmd(DMA2_Stream7, ENABLE);`  

(5) 查询DMA传输状态  
查询DMA传输通道的状态:  
`FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef * DMAy_Streamx, uint32_t DMAy_FLAG);`  
获取当前剩余数据量大小的函数:  
`uint16_t DMA_GetCurrCounter(DMA_Stream_TypeDef *DMAy_Streamx);`









#### C++嵌入C代码

```C++
#ifdef __cplusplus
        extern "C" {
        #endif
//c语法代码段
#ifdef __cplusplus
        }
        #endif //

```

`__cplusplus`是CPP中的自定义宏，则表示这是一段cpp的代码，编译器按c++的方式编译系统.。
如果这时候我们需要使用c语言的代码,那么就需要加上(extern "C" { )这一段来说明，要不编译器会把c代码按c++模式编译，会出现问题。  
C和C++对函数的处理方式是不同的.extern "C"是使C++能够调用C写作的库文件的一个手段，如果要对编译器提示使用C的方式来处理函数的话，那么就要使用extern "C"来说明。

####Q & A

> 1

```C
//uart.c中uart_init()函数
RCC->APB2ENR|=1<<4; //使能串口 1 时钟

//但是根据stm32f4xx.h中的宏

/********************  Bit definition for RCC_APB2ENR register  ***************/
#define  RCC_APB2ENR_TIM1EN                  ((uint32_t)0x00000001)
#define  RCC_APB2ENR_TIM8EN                  ((uint32_t)0x00000002)
#define  RCC_APB2ENR_USART1EN                ((uint32_t)0x00000010)
#define  RCC_APB2ENR_USART6EN                ((uint32_t)0x00000020)
#define  RCC_APB2ENR_ADC1EN                  ((uint32_t)0x00000100)

```
为什么是左移4位?有错吗?

> 2





















