module simu_random();
reg I_clk,I_rst_n;
reg I_drive,I_load;
wire [9:0] O_box_x;
wire [9:0] O_box_y;

random_box box(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_drive(I_drive),
    .I_load(I_load),
    .O_box_x(O_box_x),
    .O_box_y(O_box_y)
);

initial I_clk=0;
always #5 I_clk=~I_clk; 

initial 
begin
    I_rst_n=1'b0;I_drive=1'b0;I_load=1'b0;
#20 I_rst_n=1'b1;I_load=1'b1;
#20 I_load=1'b0;
#20 I_drive=1'b1;
#20 I_drive=1'b0;
#20 I_drive=1'b1;
#20 I_drive=1'b0;
end

endmodule