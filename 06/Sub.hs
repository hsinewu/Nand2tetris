module Sub (bin15) where

bin :: Int -> String
bin 0 = "0"
bin 1 = "1"
bin n = bin (div n 2) ++ bin (rem n 2)

bin15 n = dropped where
	bins = bin n
	padded = replicate 15 '0' ++ bins
	dropped = drop (length padded - 15) padded
