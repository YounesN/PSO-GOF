import xml.etree.ElementTree

class Parameter:
    def __init__(self, pars):
        self.name = pars['name']
        self.kind = pars['kind']
        self.start = float(pars['start'])
        self.end = float(pars['end'])
        self.pattern = pars['pattern']
        self.reference = pars['reference']
    
    def __str__(self):
        return '{} {} {} {} {} {}'.format(self.name, self.kind,
                                          str(self.start), str(self.end),
                                          self.pattern, self.reference)

class Parameters:
    def __init__(self, inputfile):
        self.parameters = []
        
        e = xml.etree.ElementTree.parse(inputfile).getroot()
        for par in e.find('parameters').findall('parameter'):
            pars = {}
            pars['name'] = par.find('name').text
            pars['kind'] = par.find('kind').text
            pars['start'] = par.find('start').text
            pars['end'] = par.find('end').text
            pars['pattern'] = par.find('pattern').text
            pars['reference'] = par.find('reference').text
            
            self.parameters.append(Parameter(pars))

    def GetDim(self):
        return len(self.parameters)
    
    def GetParameterByName(self, name):
        for par in self.parameters:
            if par.name == name:
                return par

