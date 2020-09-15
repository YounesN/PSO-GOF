import os

# Check if PREBUILT directory exists!
if(os.path.isdir('./PREBUILT')):
  # If so the run the PREBUILT method
  import prebuilt_charge
else:
  # Otherwise, run pso_general, the automated version
  import pso_general

print('All done!')
