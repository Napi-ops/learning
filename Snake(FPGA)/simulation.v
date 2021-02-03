module simulation ();
reg I_clk,I_rst_n;
reg I_load,I_drive;
reg I_button_u,I_button_d,I_button_r,I_button_l;
wire [3:0] O_red;
wire [3:0] O_green;
wire [3:0] O_blue;
wire O_hs;
wire O_vs;


top top(
    .I_clk(I_clk),
    .I_rst_n(I_rst_n),
    .I_load(I_load),
    .I_drive(I_drive),
    .I_button_u(I_button_u),
    .I_button_d(I_button_d),
    .I_button_r(I_button_r),
    .I_button_l(I_button_l),
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
    I_button_u=1'b0;I_button_d=1'b0;I_button_r=1'b0;I_button_l=1'b0;I_rst_n=1'b1;I_load=1'b1;
 #20I_drive=1'b1;I_load=1'b0;
 #20I_drive=1'b0;
 #100 $finish;
end


endmodule //simulation