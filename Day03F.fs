open System.IO

let CheckLine (x, y, dx, dy, count) (line:string) =
    let nCount = match y = 0 && line.[x] = '#' with | true -> count+1L | _ -> count
    let ny = match y+1 = dy with | true -> 0 | _ -> y + 1
    let nx = match ny = 0 with | false -> x | _ -> (x+dx) % (line.Length-1)
    (nx, ny, dx, dy, nCount)

let GetCount (_, _, _, _, count) = count

let ProcessSlope slope lines =
   Array.fold(fun state line -> CheckLine state line) slope lines |> GetCount

[<EntryPoint>]
let main argv =
    let data = File.ReadAllText(argv.[0]).Split('\n')
    let slopes = [| (0, 0, 3, 1, 0L); (0, 0, 1, 1, 0L); (0, 0, 5, 1, 0L); (0, 0, 7, 1, 0L); (0, 0, 1, 2, 0L)|]
    let results = Array.map(fun x -> ProcessSlope x data) slopes
    printfn "Part 1: %A" results.[0]
    Array.reduce(*) results |> printfn "Part 2: %A" 
    0 // return an integer exit code
