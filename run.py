import os

# Check if PREBUILT directory exists!
# If so the run the PREBUILT method
if os.path.isdir('./PREBUILT'):
    # If par-charge.xml exists, run the charged version
    if os.path.exists("par-charge.xml"):
        import prebuilt_charge
    else:
        import prebuilt
else:
    # Otherwise, run pso_general, the automated version
    import automated

print('All done!')
