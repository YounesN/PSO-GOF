import xml.etree.ElementTree

class Charge:
    def __init__(self, charge):
        self.pattern = charge['pattern']
        if charge['dependent'] == 'true':
            self.dependent = True
        else:
            self.dependent = False
            self.count = int(charge['count'])
            self.start = float(charge['start'])
            self.end = float(charge['end'])
            self.reference = float(charge['reference'])
    
    def __str__(self):
        if self.dependent:
            return '{} {}'.format(self.pattern, self.dependent)
        else:
            return '{} {} {} {} {}'.format(self.pattern, self.reference, str(self.count), str(self.start), str(self.end))

class Charges:
    def __init__(self, inputfile):
        self.charges = []

        e = xml.etree.ElementTree.parse(inputfile).getroot()

        for ch in e.find('charges').findall('charge'):
            charge = {}
            charge['pattern'] = ch.find('pattern').text
            charge['reference'] = ch.find('reference').text
            charge['dependent'] = ch.find('dependent').text
            if charge['dependent'] == 'false':
                charge['count'] = ch.find('count').text
                charge['start'] = ch.find('start').text
                charge['end'] = ch.find('end').text
            
            self.charges.append(Charge(charge))

    def GetDim(self):
        return len(self.charges)