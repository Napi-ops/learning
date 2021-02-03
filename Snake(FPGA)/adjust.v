module adjust (
    input            I_clk,
    input            I_rst_n,
    input            I_button_u, //向上按键
    input            I_button_d, //向下按键
    input            I_button_r, //向右按键
    input            I_button_l, //向左按键
    output           O_button_u, //向上按键输出
    output           O_button_d, //向下按键输出
    output           O_button_r, //向右按键输出
    output           O_button_l //向左按键输出
);

//四个计数器，分别处理四个按键
reg [3:0] cnt1;
reg [3:0] cnt2;
reg [3:0] cnt3;
reg [3:0] cnt4;

//对四个按键分别去抖动
always@(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        cnt1<= 4'b0;
    else if(I_button_u == 1'b0)
        cnt1 <= 4'h0;
    else if(I_button_u < 4'h8)
        cnt1 <= cnt1 + 1'b1;
end
assign O_button_u= cnt1[3];

always@(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        cnt2<= 4'b0;
    else if(I_button_d == 1'b0)
        cnt2 <= 4'h0;
    else if(I_button_d < 4'h8)
        cnt2 <= cnt2 + 1'b1;
end
assign O_button_d= cnt2[3];

always@(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        cnt3<= 4'b0;
    else if(I_button_r == 1'b0)
        cnt3 <= 4'h0;
    else if(I_button_r < 4'h8)
        cnt3 <= cnt3 + 1'b1;
end
assign O_button_r= cnt3[3];

always@(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        cnt4<= 4'b0;
    else if(I_button_l == 1'b0)
        cnt4 <= 4'h0;
    else if(I_button_l < 4'h8)
        cnt4 <= cnt4 + 1'b1;
end
assign O_button_l= cnt4[3];

endmodule 