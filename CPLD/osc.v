module osc(	CLK,
			RESET,
			MCU_Read,
			
			SRAM_WE_n,
			SRAM_Data,
			SRAM_Add,

			ADC_CLK,
			ADC1_Data,
			ADC2_Data,
			
			ADC_GetFreq,
			
			Trig_In,
			Sample_Finish,
			Time_Sel);

input CLK;
input RESET;
reg   RESETLast;
input MCU_Read; /* 辅助MCU读取数据，上升沿有效 */
reg   MCU_ReadLast;

output Sample_Finish;
reg    Sample_Finish;
reg [2:0]  Trig_Type;
reg    Sample_Start;
input [1:0] Trig_In;
reg   [1:0] Trig_InLast;

output SRAM_WE_n;
reg    SRAM_WE_n;
output [15:0] SRAM_Data;
reg    [15:0] SRAM_Data;    
output [17:0] SRAM_Add;
reg    [17:0] SRAM_Add;

output ADC_CLK;
reg    ADC_CLK;
input [7:0] ADC1_Data;
input [7:0] ADC2_Data;

input       ADC_GetFreq; // 读取频率，上升沿准备数据，下降沿主机读取数据
reg         ADC_GetFreqLast;
reg   [2:0] ADC_GetFreqCnt;
reg  [23:0] ADC1_Freq;
reg  [23:0] ADC2_Freq;
reg   [1:0] ADC1_FreqFlag;
reg   [1:0] ADC2_FreqFlag;
 
input[4:0] Time_Sel;
reg[23:0]  Time_Counter;
reg[23:0]  Time_CntMax;

/*always @ (posedge CLK) begin
	if(RESET == 1'B0) begin
		ADC1_Freq <= 24'D0;
		ADC2_Freq <= 24'D0;
		ADC1_FreqFlag <= 2'D0;
		ADC2_FreqFlag <= 2'D0;
	end
	else begin
		// 脉冲计数部分
		if(Trig_In[0] == 1'B1 && Trig_InLast[0] == 1'B0) begin // CH1的上升沿
			if(ADC1_FreqFlag < 2'D2) ADC1_FreqFlag <= ADC1_FreqFlag + 1'B1;
		end
		if(ADC1_FreqFlag == 1'B1) ADC1_Freq <= ADC1_Freq + 1'B1;
		
		if(Trig_In[1] == 1'B1 && Trig_InLast[1] == 1'B0) begin // CH2的上升沿
		
		end
		
		// MCU读取频率部分
		if(ADC_GetFreqLast == 1'B0 && ADC_GetFreq == 1'B1) begin // MCU的上升沿
			if(ADC1_FreqFlag == 2'D2) begin // 如果测频完成
				SRAM_Data <= ADC1_Freq[16:0];
				ADC1_FreqFlag <= ADC1_FreqFlag + 1'B1;
			end
			if(ADC1_FreqFlag == 2'D3) begin // 如果测频完成
				SRAM_Data <= ADC1_Freq[23:0];
			end
		end
	end
end*/

always @ (posedge CLK) begin
	RESETLast <= RESET;
	if(RESET == 1'B0 && RESETLast == 1'B1) begin
		case(Time_Sel)
			5'D00 : Time_CntMax <= 24'D0_000_000;
			5'D01 : Time_CntMax <= 24'D0_000_001;
			5'D02 : Time_CntMax <= 24'D0_000_002;
			5'D03 : Time_CntMax <= 24'D0_000_005;
			5'D04 : Time_CntMax <= 24'D0_000_010;
			5'D05 : Time_CntMax <= 24'D0_000_020;
			5'D06 : Time_CntMax <= 24'D0_000_050;
			5'D07 : Time_CntMax <= 24'D0_000_100;
			5'D08 : Time_CntMax <= 24'D0_000_200;
			
			5'D09 : Time_CntMax <= 24'D0_000_500;
			5'D10 : Time_CntMax <= 24'D0_001_000;
			5'D11 : Time_CntMax <= 24'D0_002_000;
			5'D12 : Time_CntMax <= 24'D0_005_000;
			5'D13 : Time_CntMax <= 24'D0_010_000;
			5'D14 : Time_CntMax <= 24'D0_020_000;
			5'D15 : Time_CntMax <= 24'D0_050_000;
			5'D16 : Time_CntMax <= 24'D0_100_000;
			5'D17 : Time_CntMax <= 24'D0_200_000;
			5'D18 : Time_CntMax <= 24'D0_500_000;
			
//			default : Time_CntMax <= 24'D1_000_000;
			default : Time_CntMax <= 24'D0_000_100;
		endcase
	end
	else begin
		case(Time_Sel)
			5'D00 : Trig_Type <= 3'D0;  //none
			5'D01 : Trig_Type <= 3'D1;  //CH1 posedge
			5'D02 : Trig_Type <= 3'D2;  //CH1 negedge
			5'D03 : Trig_Type <= 3'D3;  //CH2 posedge
			5'D04 : Trig_Type <= 3'D4;  //CH2 negedge
			default : Trig_Type <= 2'D0; //none
		endcase
	end
end

always @ (posedge CLK) begin
	Trig_InLast <= Trig_In;
	case(Trig_Type)
		3'D0 : Sample_Start <= 1'B1;
		3'D1 : if(Trig_InLast[0] == 1'B0 && Trig_In[0] == 1'B1) Sample_Start <= 1'B1;
		3'D2 : if(Trig_InLast[0] == 1'B1 && Trig_In[0] == 1'B0) Sample_Start <= 1'B1;
		3'D3 : if(Trig_InLast[1] == 1'B0 && Trig_In[1] == 1'B1) Sample_Start <= 1'B1;
		3'D4 : if(Trig_InLast[1] == 1'B1 && Trig_In[1] == 1'B0) Sample_Start <= 1'B1;
		default : Sample_Start <= 1'B1;
	endcase
	
	MCU_ReadLast <= MCU_Read;
	if(MCU_ReadLast == 1'B0 && MCU_Read == 1'B1) begin // 如果是MCU_Read的上升沿
		if(Sample_Finish == 1'B1) begin
			SRAM_Add <= SRAM_Add + 1'B1;
			SRAM_WE_n = 1'B1;
//			SRAM_WE_n = 1'B0;
//			SRAM_Data <= 0;
		end
	end
	else begin // 否则是CLK的上升沿
		if(RESET == 1'B0) begin
			Time_Counter <= 24'D0;
			Sample_Finish <= 1'B0;
			ADC_CLK <= 1'B0;
			SRAM_Add <= 18'D0;
			Sample_Start <= 1'B0;
			
			ADC1_Freq <= 24'D0;
			ADC2_Freq <= 24'D0;
			ADC1_FreqFlag <= 2'D0;
			ADC2_FreqFlag <= 2'D0;
			ADC_GetFreqCnt <= 3'D0;
		end
		else begin
			if(Sample_Finish == 1'B0 && Sample_Start == 1'B1) begin
	//			SRAM_WE_n <= 1'B1;
				if(SRAM_Add > 1023) begin // 采样完成,采了1024个点
					Sample_Finish <= 1'B1;
					SRAM_Data <= 16'BZZZZ_ZZZZ_ZZZZ_ZZZZ;
					SRAM_Add <= 18'D0; // 地址清零 
				end
				else begin // 否则继续采样 
					if(Time_Counter < Time_CntMax) Time_Counter <= Time_Counter + 1'B1;
					else begin
						Time_Counter <= 24'D0;
						ADC_CLK <= ~ADC_CLK;
						if(ADC_CLK == 1'B1) begin // 下降沿读取AD值 
							SRAM_WE_n <= 1'B0;
							SRAM_Data <= {ADC2_Data, ADC1_Data};
							SRAM_Add <= SRAM_Add + 1'B1;
						end
					end
				end
			end
			//////////////////////////////////////////////
			if(Trig_In[0] == 1'B1 && Trig_InLast[0] == 1'B0) begin // CH1的上升沿
				if(ADC1_FreqFlag < 2'D2) ADC1_FreqFlag <= ADC1_FreqFlag + 1'B1;
			end
			if(ADC1_FreqFlag == 1'B1) ADC1_Freq <= ADC1_Freq + 1'B1;
			
			if(Trig_In[1] == 1'B1 && Trig_InLast[1] == 1'B0) begin // CH2的上升沿
				if(ADC2_FreqFlag < 2'D2) ADC2_FreqFlag <= ADC2_FreqFlag + 1'B1;
			end
			if(ADC2_FreqFlag == 1'B1) ADC2_Freq <= ADC2_Freq + 1'B1;
			
			// MCU读取频率部分
			ADC_GetFreqLast <= ADC_GetFreq;
			if(ADC_GetFreqLast == 1'B0 && ADC_GetFreq == 1'B1) begin // MCU的上升沿
				ADC_GetFreqCnt <= ADC_GetFreqCnt + 1'B1;
				SRAM_WE_n <= 0;
				
				SRAM_Data <= 16'D0;
				case(ADC_GetFreqCnt)
					3'D0 : SRAM_Data <= (ADC1_FreqFlag == 2'D2) ? 16'D1 : 16'D0;
					3'D1 : SRAM_Data <= ADC1_Freq[15:0];
					3'D2 : SRAM_Data <= {8'D0, ADC1_Freq[17:16]};
					
					3'D3 : SRAM_Data <= (ADC2_FreqFlag == 2'D2) ? 16'D1 : 16'D0;
					3'D4 : SRAM_Data <= ADC2_Freq[15:0];
					3'D5 : begin
						SRAM_Data <= {8'D0, ADC2_Freq[17:16]};
						ADC_GetFreqCnt <= 3'D0;
					end
				endcase
			end
		end
	end
end

endmodule

