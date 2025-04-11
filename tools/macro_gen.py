import argparse

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
    
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_arguments()
    print("Snow depths:", args.snowDepths)
    print("Rock material:", args.rockMaterials)
    
    generated_macros = []
    for rock_material in args.rockMaterials:
        for snow_depth in args.snowDepths:
            # Create file and open it for writing
            filename = f"./run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts.mac"
            generated_macros.append(filename)
            with open(filename, 'w') as file:
                # Write the macro commands to the file
                file.write(f"/run/initialize\n")
                file.write(f"/am/outputfile run_{rock_material}_rockmaterial_{snow_depth}m_snowdepth_{args.nEvents}evts.csv\n")
                file.write(f"/snowsim/det/setRockMaterial {rock_material}\n")
                file.write(f"/snowsim/det/setRockThickness 2 m\n")
                file.write(f"/snowsim/det/setSnowDepth {snow_depth} m\n")
                file.write(f"/control/execute crysetup.mac\n")
                file.write(f"/run/beamOn {args.nEvents}\n")
                
    # Generate task spooler script
    with open("../run_scripts/submit_runs.sh", "w") as script_file:
        script_file.write("#!/bin/bash\n")
        for macro in generated_macros:
            script_file.write(f"tsp bash -c \'source /opt/software/geant4/geant4-v11.3.0-install/bin/geant4.sh && cd ../build && ./snowsim ../tools/{macro} && cd ../tools\'\n") 
