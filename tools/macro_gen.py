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
    
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_arguments()
    print("Snow depths:", args.snowDepths)
    print("Rock material:", args.rockMaterials)
    
    generated_macros = []
    generated_outputfiles = []
    for rock_material in args.rockMaterials:
        for snow_depth in args.snowDepths:
            # Create file and open it for writing
            filename = f"./{args.projectName}/run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts.mac"
            generated_macros.append(filename)
            outputfile = f"run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts.csv"
            generated_outputfiles.append(outputfile)
            Path(f"./{args.projectName}").mkdir(parents=True, exist_ok=True)
            with open(filename, 'w') as file:
                # Write the macro commands to the file
                file.write(f"/run/initialize\n")
                file.write(f"/am/outputfile {outputfile}\n")
                file.write(f"/snowsim/det/setRockMaterial {rock_material}\n")
                file.write(f"/snowsim/det/setRockThickness 2 m\n")
                file.write(f"/snowsim/det/setSnowDepth {snow_depth} m\n")
                file.write(f"/control/execute crysetup.mac\n")
                file.write(f"/run/beamOn {args.nEvents}\n")
                
    # Generate task spooler script
    Path(f"../results/{args.projectName}").mkdir(parents=True, exist_ok=True)
    with open(f"../run_scripts/submit_runs_{args.projectName}.sh", "w") as script_file:
        script_file.write("#!/bin/bash\n")
        script_file.write(f"mkdir -p ../results/{args.projectName}\n")
        for i,macro in enumerate(generated_macros):
            wildcard_output = generated_outputfiles[i].replace(".csv", "*")
            script_file.write(f"tsp bash -c \'source /opt/software/geant4/geant4-v11.3.0-install/bin/geant4.sh && cd ../build && ./snowsim ../tools/{macro} && mv {wildcard_output} ../results/{args.projectName} && cd ../tools\'\n") 
