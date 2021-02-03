module random_box (
    input             I_clk,
    input             I_rst_n,
    input             I_drive,   //盒子更新驱动信号
    output wire [9:0] O_box_x,   //生成的盒子x坐标
    output wire [9:0] O_box_y    //生成的盒子y坐标
);

wire [9:0]  rand_num;   //伪随机数
wire [9:0]  rand_x;     //传递x坐标
wire [9:0]  rand_y;     //传递y坐标

//生成伪随机数
random_num number(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .rand_num(rand_num)
);

//生成盒子x、y坐标
random_coordinate coordinate(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_drive(I_drive),
    .rand_num(rand_num),
    .rand_x(rand_x),
    .rand_y(rand_y)
);

assign O_box_x = rand_x;
assign O_box_y = rand_y;

endmodule