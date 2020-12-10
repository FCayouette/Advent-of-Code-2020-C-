open System.IO

let FindOddOne line (values:(int*int64)[]) =
    if line < 25 then false
    else let work = values.[line-25..line-1] |> Array.map(snd)
         (seq { for i in 0 .. 23 do
                for j in i+1 ..24 do work.[i] + work.[j] } |> Seq.filter(fun x -> x = (snd values.[line])) |> Seq.toArray).Length = 0

let rec FindMinMaxSequence (l, h, running) target (values:_[]) =
    if running = target then
        let s = values.[l..h]
        Array.min s + Array.max s
    else if running < target then 
        FindMinMaxSequence (l, h+1, running + values.[h]) target values
        else FindMinMaxSequence (l+1, h, running - values.[l]) target values

[<EntryPoint>]
let main argv =
    let values = File.ReadAllLines(argv.[0]) |> Array.map System.Int64.Parse
    let indexedValue = Array.indexed values
    let part1 = snd (Array.filter(fun (i, x) -> FindOddOne i indexedValue) indexedValue).[0]
    printfn "Part 1: %A" part1
    printfn "Part 2: %A" <| FindMinMaxSequence (0,0,0L) part1 values
    0 // return an integer exit code
