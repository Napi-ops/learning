module random_num(
    input           I_clk,
    input           I_rst_n,
    output reg[9:0] rand_num    //伪随机数
);

//生成伪随机数，随机数种子为111111111
always@(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)  
        rand_num <= 10'b11011101;
    else 
        begin
            rand_num[0] <= rand_num[9];
            rand_num[1] <= rand_num[0];
            rand_num[2] <= rand_num[1];
            rand_num[3] <= rand_num[2];
            rand_num[4] <= rand_num[3]^rand_num[9];
            rand_num[5] <= rand_num[4]^rand_num[9];
            rand_num[6] <= rand_num[5]^rand_num[9];
            rand_num[7] <= rand_num[6];
            rand_num[8] <= rand_num[7];
            rand_num[9] <= rand_num[8];
        end
end
endmodule
