`timescale 1ns / 1ps
module top (
    input            I_clk,
    input            I_rst_n,
    input            I_button_u, //向上按键
    input            I_button_d, //向下按键
    input            I_button_r, //向右按键
    input            I_button_l, //向左按键
    output     [3:0] O_red,      //VGA红色分量
    output     [3:0] O_green,    //VGA绿色分量
    output     [3:0] O_blue,     //VGA蓝色分量
    output           O_hs,       //VGA行同步信号
    output           O_vs,       //VGA场同步信号
    output     [7:0] O_hexplay_an,  //数码管显示地址
    output     [7:0] O_hexplay_data //数码管显示数据
);

wire       drive;   //传输盒子更新信号
wire [7:0] score;   //传输分数
wire [9:0] box_x;   //传输盒子x坐标
wire [9:0] box_y;   //传输盒子y坐标
wire [219:0] snake_body_x; //传输蛇身体x坐标
wire [219:0] snake_body_y; //传输蛇身体y坐标

//传输四个去抖动后的按键信号
wire button_u;
wire button_d;
wire button_r;
wire button_l;

//对按键信号去抖动
adjust adjustment
(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_button_u(I_button_u),
    .I_button_d(I_button_d),
    .I_button_r(I_button_r),
    .I_button_l(I_button_l),
    .O_button_u(button_u),
    .O_button_d(button_d),
    .O_button_r(button_r),
    .O_button_l(button_l)
);

//控制蛇
snake control_snake
(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_button_u(I_button_u),
    .I_button_d(I_button_d),
    .I_button_r(I_button_r),
    .I_button_l(I_button_l),
    .I_box_x(box_x),
    .I_box_y(box_y),
    .O_drive(drive),
    .O_score(score),
    .O_snake_body_x(snake_body_x),
    .O_snake_body_y(snake_body_y)
);

//随机生成盒子
random_box random_box
(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_drive(drive),
    .O_box_x(box_x),
    .O_box_y(box_y)
);

//显示得分
score show_score
(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_score(score),
    .hexplay_an(O_hexplay_an),
    .hexplay_data(O_hexplay_data)
);

//画面渲染
vga_driver vga_driver
(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_box_x(box_x),
    .I_box_y(box_y),
    .I_snake_body_x(snake_body_x),
    .I_snake_body_y(snake_body_y),
    .O_red(O_red),
    .O_green(O_green),
    .O_blue(O_blue),
    .O_hs(O_hs),
    .O_vs(O_vs)
);
endmodule