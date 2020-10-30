import xml.etree.ElementTree


class Parameter:
    def __init__(self, pars):
        self.filename = pars['filename']
        self.name = pars['name']
        self.kind = pars['kind']
        self.start = float(pars['start'])
        self.end = float(pars['end'])
        self.pattern = pars['pattern']
        self.is_charge = pars['is_charge']
        self.is_fixed = pars['is_fixed']

    def __str__(self):
        return '{} {} {} {} {} {} {} {}'.format(self.filename, self.name, self.kind, str(self.start), str(self.end),
                                                self.pattern, self.is_charge, self.is_fixed)


class Parameters:
    def __init__(self, input_file):
        self.parameters = []
        self.is_fixed_exists = False

        e = xml.etree.ElementTree.parse(input_file).getroot()
        for par in e.find('parameters').findall('parameter'):
            pars = {'filename': par.find('filename').text,
                    'name': par.find('name').text,
                    'kind': par.find('kind').text,
                    'start': par.find('start').text,
                    'end': par.find('end').text,
                    'pattern': par.find('pattern').text}
            charge_par = par.find('charge')
            if charge_par is not None:
                pars['is_charge'] = charge_par.text
            else:
                pars['is_charge'] = 'false'

            fixed_par = par.find('fixed')
            if fixed_par is not None:
                pars['is_fixed'] = charge_par.text
            else:
                pars['is_fixed'] = 'false'

            if pars['is_fixed'] == 'true':
                self.is_fixed_exists = True
            self.parameters.append(Parameter(pars))

    def get_dim(self):
        if self.is_fixed_exists:
            return len(self.parameters) - 1
        else:
            return len(self.parameters)

    def get_parameter_by_name(self, name):
        for par in self.parameters:
            if par.name == name:
                return par
