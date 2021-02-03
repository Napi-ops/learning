module simu_vga ();
reg I_clk,I_rst_n;
reg [9:0] I_box_x;
reg [9:0] I_box_y;
wire [3:0] O_red;
wire [3:0] O_green;
wire [3:0] O_blue;
wire O_hs;
wire O_vs;

vga_driver vga_driver(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_box_x(I_box_x),
    .I_box_y(I_box_y),
    .O_red(O_red),
    .O_green(O_green),
    .O_blue(O_blue),
    .O_hs(O_hs),
    .O_vs(O_vs)
);

initial I_clk=0;
always #5 I_clk=~I_clk; 

initial 
begin
    I_rst_n=1'b1;I_box_x=10'b0;I_box_y=10'b0;
#100 $finish;
end

endmodule //simu_vga