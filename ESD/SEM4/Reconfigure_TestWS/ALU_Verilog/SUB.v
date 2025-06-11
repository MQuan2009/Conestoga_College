module SUB (
    input wire EN,
    input wire CARRYIN,
    input wire [7:0] A,
    input wire [7:0] B,
    output reg [7:0] RESULT,
    output reg BORROW
);
    reg signed [8:0] A_ext, B_ext, temp;

    always @(*) begin
        if (EN) begin
            A_ext = {A[7], A};  // Sign-extend to 9 bits
            B_ext = {B[7], B};

            if (CARRYIN)
                temp = A_ext - B_ext - 1;
            else
                temp = A_ext - B_ext;

            RESULT = temp[7:0];
            BORROW = temp[8];  // MSB of result for overflow or borrow
        end else begin
            RESULT = 8'b0;
            BORROW = 1'b0;
        end
    end
endmodule

