module random_coordinate (
    input            I_clk,
    input            I_rst_n,
    input            I_drive,      //盒子更新驱动信号
    input      [9:0] rand_num,     //伪随机数
    output reg [9:0] rand_x,       //随机x坐标
    output reg [9:0] rand_y        //随机y坐标
);

reg tag;   //用以标记是否更新y坐标

//生成盒子x、y坐标
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
    begin
        rand_x <= 10'd340;
        rand_y <= 10'd332;
        tag <= 0;
    end
    else if(I_drive) 
    begin
        rand_x <= rand_num;
        if(rand_x<11'd100)
            rand_x <= rand_x + 11'd200;
        else if(rand_x > 11'd950)
            rand_x <= rand_x - 11'd150;
        tag <= 1;
    end
    else if(tag)
    begin
        rand_y <= rand_num;
        if(rand_y<11'd100)
            rand_y <= rand_y + 11'd200;
        else if(rand_y >11'd950)
            rand_y <= rand_y -11'd150;      
        tag <= 0;
    end
end

endmodule 