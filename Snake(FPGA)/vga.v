module vgaaa(
    input            I_clk, 
    input            I_rst_n,
    //input      [9:0] I_box_x,    //盒子的x坐标
    //input      [9:0] I_box_y,    //盒子的y坐标
    output reg [3:0] O_red,      //VGA红色分量
    output reg [3:0] O_green,    //VGA绿色分量
    output reg [3:0] O_blue,     //VGA蓝色分量
    output           O_hs,       //VGA行同步信号
    output           O_vs        //VGA场同步信号
);

//分辨率为1280*1024时行时序各个参数定义
parameter   C_H_Sync_Pulse = 112,
            C_H_Back_Porch = 248,
            C_H_Active_Time = 1280,
            C_H_Front_Porch = 48,
            C_H_Line_Period = 1688;

//分辨率为1280*1024时场时序各个参数定义
parameter   C_V_Sync_Pulse = 3,
            C_V_Back_Porch = 38,
            C_V_Active_Time = 1024,
            C_V_Front_Porch = 1,
            C_V_Line_Period = 1066;

//定义图像中“一格”为16*16的像素块
parameter C_Grid = 16;

reg [11:0]  R_H_cnt;    //行时序计数器
reg [11:0]  R_V_cnt;    //场时序计数器
//wire         R_Clk_108M; //108MHz时钟
//wire         locked;     //为clk_wiz_0使用

wire W_Active_Flag;     //激活标志，当信号为1时RGB数据显示在屏幕上

//产生108MHZ的时钟
// clk_wiz_0 Clk_108M
// (
//     .clk_out1(R_Clk_108M),    
//     .reset(I_rst_n), 
//     .locked(locked), 
//     .clk_in1(I_clk)
// );   

//产生行时序
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        R_H_cnt <= 12'd0;
    else if(R_H_cnt == C_H_Line_Period - 1'b1)
        R_H_cnt <= 12'd0;
    else
        R_H_cnt <= R_H_cnt + 1'b1;
end

assign O_hs = (R_H_cnt < C_H_Sync_Pulse) ? 1'b0 : 1'b1;

//产生场时序
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        R_V_cnt <= 12'd0;
    else if(R_V_cnt == C_V_Line_Period - 1'b1)
        R_V_cnt <= 12'd0;
    else if(R_H_cnt == C_H_Line_Period - 1'b1)
        R_V_cnt <= R_V_cnt + 1'b1;
    else
        R_V_cnt <= R_V_cnt;
end

assign O_vs = (R_V_cnt < C_V_Sync_Pulse) ? 1'b0 : 1'b1;

//设置激活信号
assign W_Active_Flag =  (R_H_cnt >= (C_H_Sync_Pulse + C_H_Back_Porch                  ))  &&
                        (R_H_cnt <= (C_H_Sync_Pulse + C_H_Back_Porch + C_H_Active_Time))  && 
                        (R_V_cnt >= (C_V_Sync_Pulse + C_V_Back_Porch                  ))  &&
                        (R_V_cnt <= (C_V_Sync_Pulse + C_V_Back_Porch + C_V_Active_Time))  ; 

//VGA显示
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
    begin
        O_red   <= 4'b0000;
        O_green <= 4'b0000;
        O_blue  <= 4'b0000;
    end
    else if(W_Active_Flag)
    begin
        //用红色渲染上、下、左、右边界
        if((R_H_cnt < (C_H_Sync_Pulse + C_H_Back_Porch + C_Grid))  ||
           (R_V_cnt < (C_V_Sync_Pulse + C_V_Back_Porch + C_Grid))  ||
           (R_H_cnt > (C_H_Sync_Pulse + C_H_Back_Porch + C_H_Active_Time - C_Grid))  ||
           (R_V_cnt > (C_V_Sync_Pulse + C_V_Back_Porch + C_V_Active_Time - C_Grid)))
        begin
            O_red   <= 4'b1111;
            O_green <= 4'b0000;
            O_blue  <= 4'b0000;
        end
        //用绿色渲染盒子
        // else if((R_H_cnt > C_H_Sync_Pulse +C_H_Back_Porch + I_box_x)  &&
        //         (R_V_cnt > C_V_Sync_Pulse +C_V_Back_Porch + I_box_y)  &&
        //         (R_H_cnt <= C_H_Sync_Pulse +C_H_Back_Porch + I_box_x +C_Grid)  &&
        //         (R_V_cnt <= C_V_Sync_Pulse +C_V_Back_Porch + I_box_y +C_Grid))
        // begin
        //     O_red   <= 4'b0000;
        //     O_green <= 4'b1111;
        //     O_blue  <= 4'b0000;
        // end
    end
end


endmodule