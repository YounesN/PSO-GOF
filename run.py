import os

# Check if PREBUILT directory exists!
if(os.path.isdir('./PREBUILT')):
  # If so the run the PREBUILT method
  print('PREBUILT directory detected. Running pre-built method.')
  import prebuilt
else:
  # Otherwise, run pso_general, the automated version
  print('PREBUILT directory not found. Running automated method.')
  import pso_general

print('All done!')