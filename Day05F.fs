open System.IO

let ToSeatId (x:string) =
    let binary = String.map(fun x -> match x with | 'B' | 'R' -> '1' | _-> '0') x
    Array.fold(fun id x -> match x with | '1' -> id*2 + 1 | _-> 2 * id) 0 <| binary.ToCharArray()

[<EntryPoint>]
let main argv =
    let seats = File.ReadAllLines(argv.[0]) |> Array.map(ToSeatId) |> Array.sortDescending
    printfn "Part 1: %A" seats.[0]
    let missing = Array.pairwise seats |> Array.filter(fun (h,l) -> l+1 <> h)
    printfn "Part 2: %A" (snd missing.[0] + 1)
    0 // return an integer exit code
