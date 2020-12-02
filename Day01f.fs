open System.IO

let Complement value data total =
    let comp = total - value
    let result = Array.tryFind((=) comp) data
    match result with 
    | Some x -> value * comp
    | None -> 0

let Part2 value data = 
    let comp = 2020 - value
    data |> Array.map(fun x -> Complement x data comp) |> Array.max

[<EntryPoint>]
let main argv =
    let data = File.ReadLines(argv.[0]) |> Seq.map System.Int32.Parse |> Seq.toArray
    data |> Array.map(fun x -> Complement x data 2020) |> Array.max |> printfn "Part 1: %A"
    data |> Array.map(fun x -> (Part2 x data) * x) |> Array.max |> printfn "Part 2: %A"
    0 // Returns exit code
