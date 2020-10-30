import xml.etree.ElementTree


class ParticleSwarmParameters:
    def __init__(self, input_file):
        e = xml.etree.ElementTree.parse(input_file).getroot()

        self.w = float(e.find('pso').find('w').text)
        self.c1 = float(e.find('pso').find('c1').text)
        self.c2 = float(e.find('pso').find('c2').text)