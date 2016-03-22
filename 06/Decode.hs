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

decodeComp xs = "a-comp-"

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