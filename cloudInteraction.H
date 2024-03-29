        /*-------------------ADDING CODE-----------------------*/
            Info << ">>> Avietor: ----------CLOUD COLLIDSION SECTION----------" << endl;
            // Store the particle positions
            kinematicCloud.storeGlobalPositions();

            // fluid.solve();
            fluid.correct();
            
            // Revise muc
            muc = rhoc*phase2.nu();
            
            // Evolving the cloud
            Info << ">>> Avietor: Evolving the kinematic cloud \"" << kinematicCloud.name() << "\"" << endl;
            kinematicCloud.evolve();

            // Update fluid phase volume fraction field
            Info << ">>> Avietor: Update fluid phase volume fraction field" << endl;
            alphac = max(1.0 - kinematicCloud.theta(), alphacMin);
            alphac.correctBoundaryConditions();
            alphacf = fvc::interpolate(alphac);
            alphaPhic = alphacf*phic;


            // Calculate particle and liquid phase interactions
            Info << ">>> Avietor: Calculate phase interaction (solid and liquid)" << endl;
            fvVectorMatrix cloudSU(kinematicCloud.SU(Uc));
            volVectorField cloudVolSUSu
            (
                IOobject
                (
                    "cloudVolSUSu",
                    runTime.timeName(),
                    mesh
                ),
                mesh,
                dimensionedVector
                (
                    "0",
                    cloudSU.dimensions()/dimVolume,
                    Zero
                ),
                zeroGradientFvPatchVectorField::typeName
            );    
            // Calculate SU
            cloudVolSUSu.primitiveFieldRef() = -cloudSU.source()/mesh.V();
            cloudVolSUSu.correctBoundaryConditions();
            cloudSU.source() = Zero;  
		
            Info << ">>> Avietor: ----------END CLOUD COLLIDSION----------" << endl;                  

        /*-------------------END ADDING CODE-------------------*/

