module score (
    input            I_clk,
    input            I_rst_n,
    input  [7:0]    I_score,   //得分输入
    output reg [7:0] hexplay_an,   //数码管显示地址
    output reg [7:0] hexplay_data  //数码管显示数据
);

reg [26:0] counter;   //分时复用计数器

always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
        counter <= 27'd0;
    else if(counter >= 27'd100000000)
        counter <= 27'd0;
    else
        counter <= counter + 27'd1;
end

always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
    begin
        hexplay_an <= 7'd0;
        hexplay_data <= 7'd0;
    end    
    else
    begin
        case (counter[20:18])
            3'b000:
            begin
                hexplay_an <= 8'b10111111;
                case(I_score[3:0])
                4'd0 : hexplay_data = 8'hc0; 
                4'd1 : hexplay_data = 8'hf9; 
                4'd2 : hexplay_data = 8'ha4; 
                4'd3 : hexplay_data = 8'hb0; 
                4'd4 : hexplay_data = 8'h99; 
                4'd5 : hexplay_data = 8'h92; 
                4'd6 : hexplay_data = 8'h82;
                4'd7 : hexplay_data = 8'hf8;
                4'd8 : hexplay_data = 8'h80; 
                4'd9 : hexplay_data = 8'h90; 
                endcase
            end
            3'b001:
            begin
                hexplay_an <= 8'b01111111;
                case(I_score[7:4])
                4'd0 : hexplay_data = 8'hc0; 
                4'd1 : hexplay_data = 8'hf9; 
                4'd2 : hexplay_data = 8'ha4; 
                4'd3 : hexplay_data = 8'hb0; 
                4'd4 : hexplay_data = 8'h99; 
                4'd5 : hexplay_data = 8'h92; 
                4'd6 : hexplay_data = 8'h82;
                4'd7 : hexplay_data = 8'hf8;
                4'd8 : hexplay_data = 8'h80; 
                4'd9 : hexplay_data = 8'h90; 
                endcase
            end
        endcase
    end
end
    
endmodule
