module Decode (
	bin15,
	decodeComp,
	decodeDest,
	decodeJump
) where

bin :: Int -> String
bin 0 = "0"
bin 1 = "1"
bin n = bin (div n 2) ++ bin (rem n 2)

bin15 n = dropped where
	bins = bin n
	padded = replicate 15 '0' ++ bins
	dropped = drop (length padded - 15) padded

decodeComp "0" = "0101010"
decodeComp "1" = "0111111"
decodeComp "-1" = "0111010"
decodeComp "D" = "0001100"
decodeComp "A" = "0110000"
decodeComp "M" = "1110000"
decodeComp "!D" = "0001101"
decodeComp "!A" = "0110001"
decodeComp "!M" = "1110001"
decodeComp "-D" = "0001111"
decodeComp "-A" = "0110011"
decodeComp "-M" = "1110011"
decodeComp "D+1" = "0011111"
decodeComp "A+1" = "0110111"
decodeComp "M+1" = "1110111"
decodeComp "D-1" = "0001110"
decodeComp "A-1" = "0110010"
decodeComp "M-1" = "1110010"
decodeComp "D+A" = "1110010"

decodeDest xs = [a, d, m] where
	a = if elem 'A' xs then '1' else '0'
	m = if elem 'M' xs then '1' else '0'
	d = if elem 'D' xs then '1' else '0'

decodeJump "" = "000"
decodeJump "JGT" = "001"
decodeJump "JEQ" = "010"
decodeJump "JGE" = "011"
decodeJump "JLT" = "100"
decodeJump "JNE" = "101"
decodeJump "JLE" = "110"
decodeJump "JMP" = "111"