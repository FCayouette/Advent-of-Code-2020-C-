open System.IO

let ParseLine(x:string[]) = (System.Int32.Parse x.[0],  System.Int32.Parse x.[1], x.[2].[0], x.[4])

let IsTarget x c = match c = x with | true -> 1 | _ -> 0

let Part1 (l, h, c:char, pass:string) = 
  let count = pass.ToCharArray() |> Array.map(IsTarget c) |> Array.sum
  count >= l && count <= h

let Part2 (l, h, c, pass:string) =
    match (IsTarget pass.[l-1] c) + (IsTarget pass.[h-1] c) with | 1 -> true | _ -> false

[<EntryPoint>]
let main argv =
    let data = (File.ReadAllText(argv.[0]) |> String.map(fun c -> match c with |'-' -> ' ' | ':' -> ' ' | _ -> c)).Split('\n')
    let passwords = data |> Array.map (fun (x:string) ->ParseLine (x.Split(' ')))
    Array.filter(Part1) passwords |> Array.length |> printfn "Part 1 %A" 
    Array.filter(Part2) passwords |> Array.length |> printfn "Part 2 %A"
    0 // return an integer exit code
