open System.IO

let ToIntCode (x:string) =
    if x = "nop" then 0
    else if x = "acc" then 1
         else 2

let GetInstruction (x:string) =
    let data = x.Split(' ')
    (ToIntCode data.[0], System.Int32.Parse(data.[1]))

let rec ExecuteProgram (pc, acc) (program:(_*_)[]) executed =
    if (pc = program.Length) then (acc, true)
    else match executed |> Array.tryFind(fun x -> x = pc) with
            | Some x -> (acc, false)
            | None -> let newExecuted = Array.append executed [|pc|]
                      match fst program.[pc] with | 0 -> ExecuteProgram (pc+1, acc) program newExecuted
                                                  | 1 -> ExecuteProgram (pc+1, acc + (snd (program.[pc]))) program newExecuted
                                                  | _ -> ExecuteProgram (pc+(snd program.[pc]) , acc) program newExecuted

let FixProgramLine line (program:(_*_)[]) =
    if fst program.[line] = 1 then (0, false)
    else let newProgram = program |> Array.indexed |> Array.map(fun (i,(inst, x)) -> match i = line with | false -> (inst, x)
                                                                                                         | _ -> ((match inst with | 0 -> 2 | _-> 0), x))
         ExecuteProgram (0, 0) newProgram [||]

[<EntryPoint>]
let main argv =
    let program = File.ReadAllLines(argv.[0]) |> Array.map(GetInstruction)
    printfn "Part1: %A" <| fst (ExecuteProgram (0, 0) program [||])
    let Part2 = seq{ for x in 0..program.Length-1 do FixProgramLine x program } |> Seq.filter(snd) |> Seq.map(fst) |> Seq.toArray
    printfn "Part2: %A" Part2.[0]
    0 // return an integer exit code
