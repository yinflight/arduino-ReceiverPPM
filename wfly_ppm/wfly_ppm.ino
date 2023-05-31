 /************************************************************************************************
 *    描述：
 *    用于解析天地飞PPM接收机，输出8路通道。
 *    该例程适配的是Arduino Micro 板子，使用中断引脚D3。接收机的PPM引脚接D3。其他板子按需修改。
 *    该代码基于坛友富斯控透传433的代码修改。
 *    
 *    如有问题或交流： 请加QQ群  374472178
 *    
 *    作者：Edwin 
 *    日期：2019.12.15
 ************************************************************************************************/
int channel_num = 8;  //天地飞的PPM通道数是8
unsigned long PPM_decompose[8];   //PPM信号拆分之后每个通道的值
unsigned long F_Time, FF_Time; //PPM拆分成通道时用的变量
int decompose_channel = 0; //PPM拆分时，值赋给的通道数
int print_channel = 0 , print_time = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(3, INPUT);   //设置中断引脚为输入模式
  attachInterrupt(0, Interrupt_function, FALLING);    // 开启外部中断0， 中断引脚为D3，电平下落进入中断程序
}


void loop()
{  
  //Serial.println("initsuccess");
  //调整编码，换成uint8类型，节约带宽，每5帧才发送一次，节约带宽，但是这也意味着延迟的增加，具体情况可以自行修改。
//  if (decompose_channel == print_channel && print_time == 5)
//  {
    for (int j = 0; j < channel_num; j++)
    {
     
          Serial.print(PPM_decompose[j]);
          Serial.print(" ");
      
    }
     Serial.print("\r\n");
    



//  print_time = 0;  //多少帧打印一次计数器归零
//  }
  
}



/*****************************************************
  外部中断函数。用于检测PPM信号，当低电位持续时间超过20ms，
  接下来的高电平脉冲表示第一通道。总共8个通道，以此类推。
*****************************************************/
void Interrupt_function()  //中断函数
{

  
 
  //时间读取
  F_Time = micros();
  if (F_Time >= FF_Time)
  {
    PPM_decompose[decompose_channel] = F_Time - FF_Time;
   // Serial.println(PPM_decompose[2]);
  }
  else
  {
    PPM_decompose[decompose_channel] = 4294967295 - FF_Time + F_Time;
        //Serial.println(PPM_decompose[2]);
  }
  FF_Time = F_Time;

  //判断最后一个通道
  if (PPM_decompose[decompose_channel] >= 2200)  // 2200=2.2ms 实际上只要大于2ms就行了， 但是为了保险一点，这里用的2.2ms，实际上反而容易出问题
  {
    print_channel = decompose_channel;
    decompose_channel = 0;
    print_time++;
        
  }
  else
  {
    decompose_channel++;
  }

}
