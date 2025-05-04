import argparse
from pathlib import Path

def parse_arguments():
    parser = argparse.ArgumentParser(description="Process snow depth and rock material data.")
    
    # --snowdepths takes a space-separated list of floats
    parser.add_argument(
        '--snowDepths', 
        type=float, 
        nargs='+',  # + means one or more arguments
        required=True,
        help="A space-separated list of snow depths (floats)."
    )
    
    # --rockMaterial takes a string
    parser.add_argument(
        '--rockMaterials',
        type=str,
        nargs='+',  # + means one or more arguments
        required=True,
        help="The material of the rock (string)."
    )
    
    parser.add_argument(
        '--nEvents',
        type=int,
        required=True,
        help="The number of events to simulate (integer)."
    )
    
    parser.add_argument(
        '--projectName',
        type=str,
        required=True,
        help="The name of the project (string)."
    )
    
    parser.add_argument(
        '--embedTube',
        type=bool,
        required=False,
        default=False,
        help="Whether to embed the tube in the snow (boolean)."
    )
    
    parser.add_argument(
        '--tubeHeight',
        type=float,
        required=False,
        default=0.0,
        help="The height of the tube (float)."
    )
    
    parser.add_argument(
        '--nBatches',
        type=int,
        required=False,
        default=1,
        help="The number of batches to run (integer)."
    )
    
    args = parser.parse_args()
    return args

def sort_interleave_batches(filename):
    base_part, batch_part = filename.split('_batch')
    batch_num = int(batch_part.split('.')[0]) 
    base_name = base_part 
    return (batch_num, base_name)

if __name__ == "__main__":
    args = parse_arguments()
    print("Snow depths:", args.snowDepths)
    print("Rock material:", args.rockMaterials)
    
    generated_macros = []
    generated_outputfiles = []
    for rock_material in args.rockMaterials:
        for snow_depth in args.snowDepths:
            for batch in range(args.nBatches):
                # Create file and open it for writing
                filename = f"./{args.projectName}/run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts_batch{batch}.mac"
                generated_macros.append(filename)
                outputfile = f"run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts_batch{batch}.csv"
                generated_outputfiles.append(outputfile)
                Path(f"./{args.projectName}").mkdir(parents=True, exist_ok=True)
                with open(filename, 'w') as file:
                    # Write the macro commands to the file
                    file.write(f"/run/initialize\n")
                    file.write(f"/am/outputfile {outputfile}\n")
                    if args.embedTube:
                        file.write(f"/snowsim/det/embedTube true\n")
                    file.write(f"/snowsim/det/setRockMaterial {rock_material}\n")
                    file.write(f"/snowsim/det/setRockThickness 2 m\n")
                    file.write(f"/snowsim/det/setSnowDepth {snow_depth} m\n")
                    if args.embedTube:
                        file.write(f"/snowsim/det/setTubeOffsetHeight {args.tubeHeight} m\n")
                    file.write(f"/control/execute crysetup.mac\n")
                    file.write(f"/run/beamOn {args.nEvents}\n")
    
    
    paired = list(zip(generated_macros, generated_outputfiles))
    paired_sorted = sorted(paired, key=lambda x: sort_interleave_batches(x[0]))
    generated_macros, generated_outputfiles = zip(*paired_sorted)
    generated_macros = list(generated_macros)
    generated_outputfiles = list(generated_outputfiles)
    
    # Generate task spooler script
    Path(f"../results/{args.projectName}").mkdir(parents=True, exist_ok=True)
    with open(f"../run_scripts/submit_runs_{args.projectName}.sh", "w") as script_file:
        script_file.write("#!/bin/bash\n")
        script_file.write(f"mkdir -p ../results/{args.projectName}\n")
        for i,macro in enumerate(generated_macros):
            wildcard_output = generated_outputfiles[i].replace(".csv", "*")
            script_file.write(f"tsp bash -c \'source /opt/software/geant4/geant4-v11.3.0-install/bin/geant4.sh && cd ../build && ./snowsim ../tools/{macro} && mv {wildcard_output} ../results/{args.projectName} && cd ../tools\'\n") 
