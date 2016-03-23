module Decode (
	decode
) where

--decode A instruction
decode ('@':xs) = '0' : bin15 (read xs::Int)
--decode C instruction
decode xs = "111" ++ comp ++ dest ++ jump where
	hasDest = elem '=' xs
	hasJump = elem ';' xs
	comp = decodeComp compCode where
		compCode = takeWhile (/= ';') skipDest
		skipDest = if hasDest then tail (dropWhile (/= '=') xs) else xs
	dest = decodeDest destCode where
		destCode = if hasDest then takeWhile (/= '=') xs else ""
	jump = decodeJump jumpCode where
		jumpCode = if hasJump then tail (dropWhile (/= ';') xs) else ""

bin :: Int -> String
bin 0 = "0"
bin 1 = "1"
bin n = bin (div n 2) ++ bin (rem n 2)

bin15 n = dropped where
	bins = bin n
	padded = replicate 15 '0' ++ bins
	dropped = drop (length padded - 15) padded

decodeComp xs = case xs of
	"0"   -> "0101010"
	"1"   -> "0111111"
	"-1"  -> "0111010"
	"D"   -> "0001100"
	"A"   -> "0110000"
	"M"   -> "1110000"
	"!D"  -> "0001101"
	"!A"  -> "0110001"
	"!M"  -> "1110001"
	"-D"  -> "0001111"
	"-A"  -> "0110011"
	"-M"  -> "1110011"
	"D+1" -> "0011111"
	"1+D" -> "0011111"
	"A+1" -> "0110111"
	"1+A" -> "0110111"
	"M+1" -> "1110111"
	"1+M" -> "1110111"
	"D-1" -> "0001110"
	"A-1" -> "0110010"
	"M-1" -> "1110010"
	"D+A" -> "0000010"
	"A+D" -> "0000010"
	"D+M" -> "1000010"
	"M+D" -> "1000010"
	"D-A" -> "0010011"
	"D-M" -> "1010011"
	"A-D" -> "0000111"
	"M-D" -> "1000111"
	"D&A" -> "0000000"
	"A&D" -> "0000000"
	"D&M" -> "1000000"
	"M&D" -> "1000000"
	"D|A" -> "0010101"
	"A|D" -> "0010101"
	"D|M" -> "1010101"
	"M|D" -> "1010101"

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