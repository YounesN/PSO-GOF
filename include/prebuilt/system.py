class System:
    def __init__(self, inputfile):
        e = xml.etree.ElementTree.parse(inputfile).getroot()
        
        self.molname = e.find('system').find('molname').text
        self.molname_pattern = e.find('system').find('molname').get('pattern')
        self.resname = e.find('system').find('resname').text
        self.resname_pattern = e.find('system').find('resname').get('pattern')