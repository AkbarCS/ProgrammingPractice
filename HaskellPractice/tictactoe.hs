import Data.Char
import Data.List
import System.IO
import System.Random hiding (next)

-- Basic declarations

size :: Int
size = 3

depth :: Int
depth = 9

type Grid = [[Player]]

data Player = O | B | X
              deriving (Eq, Ord, Show)

data Tree a = Node a [Tree a]
              deriving Show

-- Example winning grid
-- [[B,O,O],[O,X,O],[X,X,X]] :: Grid

next :: Player -> Player
next O = X
next X = O
next B = B

-- Grid utils

empty :: Grid
empty = replicate size (replicate size B)

full :: Grid -> Bool
full = all (/= B) . concat

-- Assume player O goes first
turn :: Grid -> Player
turn g = if os <= xs then O else X
  where
    os = length (filter (== O) ps)
    xs = length (filter (== X) ps)
    ps = concat g

wins :: Player -> Grid -> Bool
wins p g = any line (rows ++ cols ++ dias)
  where
    line = all (== p)
    rows = g
    cols = transpose g
    dias = [diag g, diag (map reverse g)]

diag :: Grid -> [Player]
diag g = [g !! n !! n | n <- [0..size-1]]

won :: Grid -> Bool
won g = wins O g || wins X g

-- Display grid

cls :: IO ()
cls = putStr "\ESC[2J"

goto :: (Int,Int) -> IO ()
goto (x,y) = putStr ("\ESC[" ++ show y ++ ";" ++ show x ++ "H")

putGrid :: Grid -> IO ()
putGrid = putStrLn . unlines . concat . interleave bar . map showRow
  where bar = [replicate ((size*4)-1) '-']

showRow :: [Player] -> [String]
showRow = beside . interleave bar . map showPlayer
  where
    beside = foldr1 (zipWith (++))
    bar    = replicate 3 "|"

showPlayer :: Player -> [String]
showPlayer O = ["   ", " O ", "   "]
showPlayer B = ["   ", "   ", "   "]
showPlayer X = ["   ", " X ", "   "]

interleave :: a -> [a] -> [a]
interleave x []     = []
interleave x [y]    = [y]
interleave x (y:ys) = y : x : interleave x ys

-- Making a move

valid :: Grid -> Int -> Bool
valid g i = 0 <= i && i < size ^ 2 && concat g !! i == B

move :: Grid -> Int -> Player -> [Grid]
move g i p =
    if valid g i then [chop size (xs ++ [p] ++ ys)] else []
    where (xs,B:ys) = splitAt i (concat g)

chop :: Int -> [a] -> [[a]]
chop n [] = []
chop n xs = take n xs : chop n (drop n xs)


-- Reading a number 

getNat :: String -> IO Int
getNat prompt = do 
    putStr prompt
    xs <- getLine
    if xs /= [] && all isDigit xs then
        return (read xs)
    else
        do putStrLn "ERROR: Invalid number"
           getNat prompt

-- Human vs. human

tictactoe :: IO ()
tictactoe = run empty O

run :: Grid -> Player -> IO ()
run g p = do
    cls
    goto (1,1)
    putGrid g
    run' g p

run' :: Grid -> Player -> IO ()
run' g p | wins O g = putStrLn "Player O wins!\n"
         | wins X g = putStrLn "Player X wins!\n"
         | full g   = putStrLn "It's a draw!\n"
         | otherwise = 
            do i <- getNat (prompt p)
               case move g i p of
                  [] -> do putStrLn "Error: Invalid move"
                           run' g p
                  [g'] -> run g' (next p)

prompt :: Player -> String
prompt p = "Player " ++ show p ++ ", enter your move:"

-- Game Trees

gametree :: Grid -> Player -> Tree Grid
gametree g p = Node g [gametree g' (next p) | g' <- moves g p]

moves :: Grid -> Player -> [Grid]
moves g p | won g = []
          | full g = []
          | otherwise = concat [move g i p | i <- [0..((size^2)-1)]]

-- Tree pruning

prune :: Int -> Tree a -> Tree a
prune 0 (Node x _) = Node x []
prune n (Node x ts) = Node x [prune (n-1) t | t <- ts]

-- Minimax algorithm

minimax :: Tree Grid -> Tree (Grid, Player)
minimax (Node g []) | wins O g  = Node (g,O) []
                    | wins X g  = Node (g,X) []
                    | otherwise = Node (g,B) []
minimax (Node g ts) | turn g == O = Node (g, minimum ps) ts'
                    | turn g == X = Node (g, maximum ps) ts'
                      where
                        ts' = map minimax ts
                        ps = [p | Node (_, p) _ <- ts']

bestmove :: Grid -> Player -> Grid
bestmove g p = head [g' | Node (g',p') _ <- ts, p' == best]
               where
                 tree             = prune depth (gametree g p)
                 Node (_,best) ts = minimax tree

-- Human vs. Computer
                 
main :: IO ()
main = do hSetBuffering stdout NoBuffering
          play empty O

play :: Grid -> Player -> IO ()
play g p  = do cls
               goto (1,1)
               putGrid g
               play' g p

play' :: Grid -> Player -> IO ()
play' g p
  | wins O g  = putStrLn "Player O wins!\n"
  | wins X g  = putStrLn "Player X wins!\n"
  | full g    = putStrLn "It's a draw!\n"
  | p == O    = do i <- getNat (prompt p)
                   case move g i p of 
                     [] -> do putStrLn "Error: Invalid move"
                              play' g p
                     [g'] -> play g' (next p)
  | p == X    = do putStr ("Player X is thinking...")
                   (play $! (bestmove g p)) (next p)

-- Exercise 1

numberOfNodesInGameTree :: Int
numberOfNodesInGameTree = numberOfNodesInTree(gametree empty O)

numberOfNodesInTree :: Tree a -> Int
numberOfNodesInTree (Node _ xs) = 1 + sum [numberOfNodesInTree x | x <- xs]

maxTreeDepthOfGameTree :: Int
maxTreeDepthOfGameTree = maxDepthOfTree(gametree empty O) 0

maxDepthOfTree :: Tree a -> Int -> Int
maxDepthOfTree (Node _ []) n = n
maxDepthOfTree (Node _ xs) n = maximum [maxDepthOfTree x n+1 | x <- xs]

-- Exercise 2

bestmoves :: Grid -> Player -> [Grid]
bestmoves g p = [g' | Node (g',p') _ <- ts, p' == best]
                where
                 tree             = prune depth (gametree g p)
                 Node (_,best) ts = minimax tree

mainEx2 :: IO ()
mainEx2 = do hSetBuffering stdout NoBuffering
             playEx2 empty O

playEx2 :: Grid -> Player -> IO ()
playEx2 g p  = do cls
                  goto (1,1)
                  putGrid g
                  playEx2' g p

playEx2' :: Grid -> Player -> IO ()
playEx2' g p
  | wins O g  = putStrLn "Player O wins!\n"
  | wins X g  = putStrLn "Player X wins!\n"
  | full g    = putStrLn "It's a draw!\n"
  | p == O    = do i <- getNat (prompt p)
                   case move g i p of 
                     [] -> do putStrLn "Error: Invalid move"
                              playEx2' g p
                     [g'] -> playEx2 g' (next p)
  | p == X    = do putStr "Player X is thinking..."
                   let gs = bestmoves g p
                   n <- randomRIO (0, length gs - 1)
                   playEx2 (gs !! n) (next p)

-- Exercise 3

minDepth :: Tree a -> Int
minDepth (Node _ []) = 0
minDepth (Node _ xs) = 1 + minimum [minDepth x | x <- xs]

bestmoveEx3 :: Grid -> Player -> Grid
bestmoveEx3 g p = head [g' | Node (g',p') _ <- sortOn minDepth ts, p' == best]
                  where
                   tree             = prune depth (gametree g p)
                   Node (_,best) ts = minimax tree

mainEx3 :: IO ()
mainEx3 = do hSetBuffering stdout NoBuffering
             playEx3 empty O

playEx3 :: Grid -> Player -> IO ()
playEx3 g p  = do cls
                  goto (1,1)
                  putGrid g
                  playEx3' g p

playEx3' :: Grid -> Player -> IO ()
playEx3' g p
  | wins O g  = putStrLn "Player O wins!\n"
  | wins X g  = putStrLn "Player X wins!\n"
  | full g    = putStrLn "It's a draw!\n"
  | p == O    = do i <- getNat (prompt p)
                   case move g i p of
                     [] -> do putStrLn "Error: Invalid move"
                              playEx3' g p
                     [g'] -> playEx3 g' (next p)
  | p == X    = do putStr "Player X is thinking..."
                   (play $! (bestmoveEx3 g p)) (next p)

-- Exercises 4c + 4d
-- Using solutions from: https://github.com/singleheart/programming-in-haskell/tree/master/ch11
-- About Alpha-Beta pruning: https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning

minimaxABEx4 :: Tree Grid -> Tree (Grid, Player)
minimaxABEx4 (Node g [])
  | wins O g = Node (g,O) []
  | wins X g = Node (g,X) []
  | otherwise = Node (g,B) []
minimaxABEx4 (Node g (t:ts))
  | turn' == cp = Node (g,cp) []
  | null ts = minimaxABEx4 t
  | otherwise = Node (g, extremal turn' ps) ts'
  where
    turn' = turn g
    Node (_,cp) _ = minimaxABEx4 t
    extremal p =
      if p == O
        then minimum
        else maximum
    ts' = map minimaxABEx4 (t:ts)
    ps = [p | Node (_,p) _ <- ts']

bestmoveEx4 :: Grid -> Player -> Tree Grid -> Grid
bestmoveEx4 g p gt = head [g' | Node (g',p') _ <- ts, p' == best]
                     where
                       tree             = findNode g gt
                       Node (_,best) ts = minimaxABEx4 tree

findNode :: Eq a => a -> Tree a -> Tree a
findNode x t = findNode' x t []

findNode' :: Eq a => a -> Tree a -> [Tree a] -> Tree a
findNode' x (Node y []) [] = Node y []
findNode' x (Node y []) (sb1:sbs) =
  if x == y
    then Node y []
    else findNode' x sb1 sbs
findNode' x (Node y (t:ts)) [] =
  if x == y
    then Node y (t : ts)
    else findNode' x t ts
findNode' x (Node y (t:ts)) (sb1:sbs)
  | x == y = Node y (t : ts)
  | x == child = Node child ct
  | otherwise = Node sibling st
  where
    Node child ct = findNode' x t ts
    Node sibling st = findNode' x sb1 sbs

mainEx4 :: IO ()
mainEx4 = do hSetBuffering stdout NoBuffering
             playEx4 empty O

playEx4 :: Grid -> Player -> IO ()
playEx4 g p  = do cls
                  goto (1,1)
                  putGrid g
                  playEx4' g p (gametree g p)

playEx4' :: Grid -> Player -> Tree Grid -> IO ()
playEx4' g p gt
  | wins O g  = putStrLn "Player O wins!\n"
  | wins X g  = putStrLn "Player X wins!\n"
  | full g    = putStrLn "It's a draw!\n"
  | p == O    = do i <- getNat (prompt p)
                   case move g i p of 
                     [] -> do putStrLn "Error: Invalid move"
                              playEx4' g p gt
                     [g'] -> playEx4 g' (next p)
  | p == X    = do putStr "Player X is thinking..."
                   (play $! (bestmoveEx4 g p gt)) (next p)