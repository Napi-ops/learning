module snake (
    input            I_clk,
    input            I_rst_n,
    input            I_button_u, //向上按键
    input            I_button_d, //向下按键
    input            I_button_r, //向右按键
    input            I_button_l, //向左按键
    input   [9:0]    I_box_x,    //盒子的x坐标
    input   [9:0]    I_box_y,    //盒子的y坐标
    output  reg      O_drive,    //更新盒子坐标信号
    output  reg [7:0] O_score,   //记录分数
    output  reg [219:0]   O_snake_body_x,   //蛇身渲染x坐标
    output  reg [219:0]   O_snake_body_y    //蛇身渲染y坐标
);

//蛇的四个方向的参数定义
parameter up = 2'b00;
parameter down = 2'b01;
parameter right = 2'b10;
parameter left = 2'b11;

//蛇的速度的参数定义
parameter snake_speed_1 = 30'd20000000;
parameter snake_speed_2 = 30'd10000000;

reg         dead_tag;           //蛇死亡标志
reg         dead;               //蛇死亡信号,为1则表示死亡
reg [1:0]   snake_dir;          //寄存蛇的方向
reg [29:0]  counter;            //计数器，用于控制蛇的速度  
reg [4:0]   snake_length;       //寄存蛇长度，从0开始计数，初始为3，最大为19
reg [10:0]  snake_body_x[19:0]; //寄存蛇的身体的x坐标
reg [10:0]  snake_body_y[19:0]; //寄存蛇的身体的y坐标

//控制蛇速度的计数器
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
        counter <= 19'b0;
    else if(dead)
        counter <= 19'b0;
    else if((snake_length < 5'd8) && (counter >= snake_speed_1))
        counter <= 19'b0;
    else if((snake_length >= 5'd8) &&  (counter >= snake_speed_2))
        counter <= 19'b0;
    else
        counter <= counter + 19'b1;
end

//控制蛇头方向的状态机
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
        snake_dir <= up;
    else if(dead)
        snake_dir <= up;
    else if(I_button_u && snake_dir != up && snake_dir != down)
        snake_dir <= up;
    else if(I_button_d && snake_dir != down && snake_dir != up)
        snake_dir <= down;
    else if(I_button_r && snake_dir != right && snake_dir != left)
        snake_dir <= right;
    else if(I_button_l && snake_dir != left && snake_dir != right)
        snake_dir <= left;
end

//控制蛇头移动，并检测是否撞墙
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
    begin
        snake_body_x[0] <= 11'd640;
        snake_body_y[0] <= 11'd512;
    end
    else if(dead && dead_tag)
    begin
        snake_body_x[0] <= 11'd640;
        snake_body_y[0] <= 11'd512;
        dead_tag <= 1'b0;
    end
    else if(((snake_length < 5'd8) && (counter == snake_speed_1)) ||
            ((snake_length >= 5'd8) && (counter == snake_speed_2)))
    begin
        case (snake_dir)
            up:
            begin
                if(snake_body_y[0]==11'd62)
                    //snake_body_y[0]<=11'd962;
                   dead_tag <= 1'b1;
               else
                   snake_body_y[0] <= snake_body_y[0] - 11'd15;
            end
            down:
            begin
                if(snake_body_y[0]==11'd962)
                    //snake_body_y[0]<=11'd62;
                   dead_tag <= 1'b1;
               else
                   snake_body_y[0] <= snake_body_y[0] +11'd15;
            end
            right:
            begin
                if(snake_body_x[0]==11'd1210)
                   //snake_body_x[0]<=11'd70;
                   dead_tag <= 1'b1;
               else
                   snake_body_x[0] <= snake_body_x[0] +11'd15;
            end
            left:
            begin
                if(snake_body_x[0]==11'd70)
                    //snake_body_x[0]<=11'd1210;
                   dead_tag <= 1'b1;
               else
                   snake_body_x[0] <= snake_body_x[0] - 11'd15;
            end
        endcase
    end
end

//控制蛇身移动
always @(posedge I_clk or negedge I_rst_n) 
begin
    if(!I_rst_n)
    begin
        snake_body_x[1] <= 11'd640; snake_body_y[1] <= 11'd527;
        snake_body_x[2] <= 11'd640; snake_body_y[2] <= 11'd542;
        snake_body_x[3] <= 11'd640; snake_body_y[3] <= 11'd557;
        snake_body_x[4] <= 11'b0; snake_body_y[4] <= 11'b0;
        snake_body_x[5] <= 11'b0; snake_body_y[5] <= 11'b0;
        snake_body_x[6] <= 11'b0; snake_body_y[6] <= 11'b0;
        snake_body_x[7] <= 11'b0; snake_body_y[7] <= 11'b0;
        snake_body_x[8] <= 11'b0; snake_body_y[8] <= 11'b0;
        snake_body_x[9] <= 11'b0; snake_body_y[9] <= 11'b0;
        snake_body_x[10] <= 11'b0; snake_body_y[10] <= 11'b0;
        snake_body_x[11] <= 11'b0; snake_body_y[11] <= 11'b0;
        snake_body_x[12] <= 11'b0; snake_body_y[12] <= 11'b0;
        snake_body_x[13] <= 11'b0; snake_body_y[13] <= 11'b0;
        snake_body_x[14] <= 11'b0; snake_body_y[14] <= 11'b0;
        snake_body_x[15] <= 11'b0; snake_body_y[15] <= 11'b0;
        snake_body_x[16] <= 11'b0; snake_body_y[16] <= 11'b0;
        snake_body_x[17] <= 11'b0; snake_body_y[17] <= 11'b0;
        snake_body_x[18] <= 11'b0; snake_body_y[18] <= 11'b0;
        snake_body_x[19] <= 11'b0; snake_body_y[19] <= 11'b0;
    end
    else if(dead)
    begin
        snake_body_x[1] <= 11'd640; snake_body_y[1] <= 11'd513;
        snake_body_x[2] <= 11'd640; snake_body_y[2] <= 11'd528;
        snake_body_x[3] <= 11'd640; snake_body_y[3] <= 11'd543;
        snake_body_x[4] <= 11'b0; snake_body_y[4] <= 11'b0;
        snake_body_x[5] <= 11'b0; snake_body_y[5] <= 11'b0;
        snake_body_x[6] <= 11'b0; snake_body_y[6] <= 11'b0;
        snake_body_x[7] <= 11'b0; snake_body_y[7] <= 11'b0;
        snake_body_x[8] <= 11'b0; snake_body_y[8] <= 11'b0;
        snake_body_x[9] <= 11'b0; snake_body_y[9] <= 11'b0;
        snake_body_x[10] <= 11'b0; snake_body_y[10] <= 11'b0;
        snake_body_x[11] <= 11'b0; snake_body_y[11] <= 11'b0;
        snake_body_x[12] <= 11'b0; snake_body_y[12] <= 11'b0;
        snake_body_x[13] <= 11'b0; snake_body_y[13] <= 11'b0;
        snake_body_x[14] <= 11'b0; snake_body_y[14] <= 11'b0;
        snake_body_x[15] <= 11'b0; snake_body_y[15] <= 11'b0;
        snake_body_x[16] <= 11'b0; snake_body_y[16] <= 11'b0;
        snake_body_x[17] <= 11'b0; snake_body_y[17] <= 11'b0;
        snake_body_x[18] <= 11'b0; snake_body_y[18] <= 11'b0;
        snake_body_x[19] <= 11'b0; snake_body_y[19] <= 11'b0; 
    end
    else if(((snake_length < 5'd8) && (counter == snake_speed_1)) ||
            ((snake_length >= 5'd8) && (counter == snake_speed_2)))
    begin
        snake_body_x[1] <= snake_body_x[0]; snake_body_y[1] <= snake_body_y[0];
        snake_body_x[2] <= snake_body_x[1]; snake_body_y[2] <= snake_body_y[1];
        snake_body_x[3] <= snake_body_x[2]; snake_body_y[3] <= snake_body_y[2];
        snake_body_x[4] <= snake_body_x[3]; snake_body_y[4] <= snake_body_y[3];
        snake_body_x[5] <= snake_body_x[4]; snake_body_y[5] <= snake_body_y[4];
        snake_body_x[6] <= snake_body_x[5]; snake_body_y[6] <= snake_body_y[5];
        snake_body_x[7] <= snake_body_x[6]; snake_body_y[7] <= snake_body_y[6];
        snake_body_x[8] <= snake_body_x[7]; snake_body_y[8] <= snake_body_y[7];
        snake_body_x[9] <= snake_body_x[8]; snake_body_y[9] <= snake_body_y[8];
        snake_body_x[10] <= snake_body_x[9]; snake_body_y[10] <= snake_body_y[9];
        snake_body_x[11] <= snake_body_x[10]; snake_body_y[11] <= snake_body_y[10];
        snake_body_x[12] <= snake_body_x[11]; snake_body_y[12] <= snake_body_y[11];
        snake_body_x[13] <= snake_body_x[12]; snake_body_y[13] <= snake_body_y[12];
        snake_body_x[14] <= snake_body_x[13]; snake_body_y[14] <= snake_body_y[13];
        snake_body_x[15] <= snake_body_x[14]; snake_body_y[15] <= snake_body_y[14];
        snake_body_x[16] <= snake_body_x[15]; snake_body_y[16] <= snake_body_y[15];
        snake_body_x[17] <= snake_body_x[16]; snake_body_y[17] <= snake_body_y[16];
        snake_body_x[18] <= snake_body_x[17]; snake_body_y[18] <= snake_body_y[17];
        snake_body_x[19] <= snake_body_x[18]; snake_body_y[19] <= snake_body_y[18];
    end
end

//蛇长度检测,并记录分数
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
    begin
        snake_length <= 5'd3;
        O_drive <= 1'b0;
        O_score <= 1'b0;
    end
    else if(dead)
    begin
        snake_length <= 5'd3;
        O_drive <= 1'b0;
        O_score <= 1'b0;
    end
    else if(O_drive)
        O_drive <= 1'b0;
    else if(((snake_length < 5'd8) && (counter == snake_speed_1)) ||
            ((snake_length >= 5'd8) && (counter == snake_speed_2)))
    begin
        if((snake_body_x[0] >= (I_box_x - 10'd7)) &&
           (snake_body_y[0] >= (I_box_y - 10'd7)) &&
           (snake_body_x[0] <= (I_box_x + 10'd7)) &&
           (snake_body_y[0] <= (I_box_y + 10'd7)))
        begin
            O_drive <= 1'b1;
            if(O_score[3:0] == 4'd9)
            begin
                O_score[3:0] <= 4'd0;
                if(O_score[7:4] == 4'd9)
                    O_score[7:4] <= 4'd0;
                else
                    O_score[7:4] <= O_score[7:4] + 4'd1;
            end
            else
                O_score[3:0] <= O_score[3:0] + 4'd1;
            if(snake_length < 5'd19)
                snake_length <= snake_length + 1'b1;
            else
                snake_length <= snake_length;
        end
    end
end

//检测蛇是否吃到自身
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        dead <= 1'b0;
    else if(dead == 1'b1)
        dead <= 1'b0;
     else if(dead_tag == 1'b1)
         dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[1]) && (snake_body_y[0] == snake_body_y[1]) && (snake_length > 5'd0))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[2]) && (snake_body_y[0] == snake_body_y[2]) && (snake_length > 5'd1))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[3]) && (snake_body_y[0] == snake_body_y[3]) && (snake_length > 5'd2))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[4]) && (snake_body_y[0] == snake_body_y[4]) && (snake_length > 5'd3))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[5]) && (snake_body_y[0] == snake_body_y[5]) && (snake_length > 5'd4))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[6]) && (snake_body_y[0] == snake_body_y[6]) && (snake_length > 5'd5))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[7]) && (snake_body_y[0] == snake_body_y[7]) && (snake_length > 5'd6))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[8]) && (snake_body_y[0] == snake_body_y[8]) && (snake_length > 5'd7))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[9]) && (snake_body_y[0] == snake_body_y[9]) && (snake_length > 5'd8))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[10]) && (snake_body_y[0] == snake_body_y[10]) && (snake_length > 5'd9))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[11]) && (snake_body_y[0] == snake_body_y[11]) && (snake_length > 5'd10))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[12]) && (snake_body_y[0] == snake_body_y[12]) && (snake_length > 5'd11))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[13]) && (snake_body_y[0] == snake_body_y[13]) && (snake_length > 5'd12))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[14]) && (snake_body_y[0] == snake_body_y[14]) && (snake_length > 5'd13))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[15]) && (snake_body_y[0] == snake_body_y[15]) && (snake_length > 5'd14))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[16]) && (snake_body_y[0] == snake_body_y[16]) && (snake_length > 5'd15))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[17]) && (snake_body_y[0] == snake_body_y[17]) && (snake_length > 5'd16))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[18]) && (snake_body_y[0] == snake_body_y[18]) && (snake_length > 5'd17))
        dead <= 1'b1;
    else if((snake_body_x[0] == snake_body_x[19]) && (snake_body_y[0] == snake_body_y[19]) && (snake_length > 5'd18))
        dead <= 1'b1;
end

//蛇身渲染信号
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
    begin
        O_snake_body_x <= ~220'b0;
        O_snake_body_y <= ~220'b0;
    end
    else if(dead)
    begin
        O_snake_body_x <= ~220'b0;
        O_snake_body_y <= ~220'b0;   
    end
    else
    begin
        O_snake_body_x[10:0] <= snake_body_x[0]; O_snake_body_y[10:0] <= snake_body_y[0];
        O_snake_body_x[21:11] <= snake_body_x[1]; O_snake_body_y[21:11] <= snake_body_y[1];
        O_snake_body_x[32:22] <= snake_body_x[2]; O_snake_body_y[32:22] <= snake_body_y[2];
        O_snake_body_x[43:33] <= snake_body_x[3]; O_snake_body_y[43:33] <= snake_body_y[3];
        if(snake_length > 5'd3)
            O_snake_body_x[54:44] <= snake_body_x[4]; O_snake_body_y[54:44] <= snake_body_y[4];
        if(snake_length > 5'd4)
            O_snake_body_x[65:55] <= snake_body_x[5]; O_snake_body_y[65:55] <= snake_body_y[5];
        if(snake_length > 5'd5)
            O_snake_body_x[76:66] <= snake_body_x[6]; O_snake_body_y[76:66] <= snake_body_y[6];
        if(snake_length > 5'd6)
            O_snake_body_x[87:77] <= snake_body_x[7]; O_snake_body_y[87:77] <= snake_body_y[7];
        if(snake_length > 5'd7)
            O_snake_body_x[98:88] <= snake_body_x[8]; O_snake_body_y[98:88] <= snake_body_y[8];
        if(snake_length > 5'd8)
            O_snake_body_x[109:99] <= snake_body_x[9]; O_snake_body_y[109:99] <= snake_body_y[9];
        if(snake_length > 5'd9)
            O_snake_body_x[120:110] <= snake_body_x[10]; O_snake_body_y[120:110] <= snake_body_y[10];
        if(snake_length > 5'd10)
            O_snake_body_x[131:121] <= snake_body_x[11]; O_snake_body_y[131:121] <= snake_body_y[11];
        if(snake_length > 5'd11)
            O_snake_body_x[142:132] <= snake_body_x[12]; O_snake_body_y[142:132] <= snake_body_y[12];
        if(snake_length > 5'd12)
            O_snake_body_x[153:143] <= snake_body_x[13]; O_snake_body_y[153:143] <= snake_body_y[13];
        if(snake_length > 5'd13)
            O_snake_body_x[164:154] <= snake_body_x[14]; O_snake_body_y[164:154] <= snake_body_y[14];
        if(snake_length > 5'd14)
            O_snake_body_x[175:165] <= snake_body_x[15]; O_snake_body_y[175:165] <= snake_body_y[15];
        if(snake_length > 5'd15)
            O_snake_body_x[186:176] <= snake_body_x[16]; O_snake_body_y[186:176] <= snake_body_y[16];
        if(snake_length > 5'd16)
            O_snake_body_x[197:187] <= snake_body_x[17]; O_snake_body_y[197:187] <= snake_body_y[17];
        if(snake_length > 5'd17)
            O_snake_body_x[208:198] <= snake_body_x[18]; O_snake_body_y[208:198] <= snake_body_y[18];
        if(snake_length > 5'd18)
            O_snake_body_x[219:209] <= snake_body_x[19]; O_snake_body_y[219:209] <= snake_body_y[19];
    end
end

endmodule 