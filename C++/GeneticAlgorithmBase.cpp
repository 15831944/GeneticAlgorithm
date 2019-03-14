#include "GeneticAlgorithmBase.h"
#include <random>
#include <time.h>

CGeneticAlgorithmBase::CGeneticAlgorithmBase(int *poprange,int nrangelen,double dcross_rate, double dmutate_rate, int npopulation, int niterations, int nDNA_size,int nTerminateSec )
    :m_poprange(poprange),m_nrangelen(nrangelen),m_dcross_rate(dcross_rate),m_dmutate_rate(dmutate_rate),m_npopulation(npopulation),m_niterations(niterations),m_nDNA_size(nDNA_size),m_nTerminateSec(nTerminateSec)
{
	m_poprange = new (std::nothrow) int[nrangelen];
	memcpy(m_poprange,poprange,nrangelen*sizeof(int));
}


CGeneticAlgorithmBase::~CGeneticAlgorithmBase()
{
	if ( m_poprange )
		delete []m_poprange;
}

bool CGeneticAlgorithmBase::InitPopulation() 
{

	m_spop = new (std::nothrow) Chromosome[m_npopulation];
	if ( m_spop == NULL )
		return false;
	srand((unsigned)time(NULL));
	for ( int i = 0; i < m_npopulation; i++ )
	{
		m_spop[i].Init(m_nDNA_size);
		for (int j = 0; j < m_nDNA_size; j++)
		{
			int xh = rand()%(m_nrangelen);
			m_spop[i].bit[j] = m_poprange[xh];
			m_spop[i].fitness = 1.0;
		}
	}
	return true;
}


bool CGeneticAlgorithmBase::Cross()
{
	int i = 0, j = 0;
	double rate_rand = 0.0;
	short int bit_temp = 0;
	int num1_rand = 0, num2_rand = 0, position_rand = 0;
	//�������������
	srand((unsigned)time(NULL));
	//Ӧ�����������ٴ��أ����趨Ϊ��Ⱥ����
	for (j = 0; j<m_npopulation; j++)
	{
		rate_rand = (double)rand() / (RAND_MAX);
		//������ڽ�����ʾͽ��н������
		if (rate_rand <= m_dcross_rate)
		{
			//�����������Ⱦɫ��
			num1_rand = (int)rand() % (m_npopulation);
			num2_rand = (int)rand() % (m_npopulation);
			//�����������Ⱦɫ��Ľ���λ��
			position_rand = (int)rand() % (m_nDNA_size);
			//���õ��㽻�棬�����֮���λ������
			for (i = position_rand; i<m_nDNA_size; i++)
			{
				bit_temp = m_spop[num1_rand].bit[i];
				m_spop[num1_rand].bit[i] = m_spop[num2_rand].bit[i];
				m_spop[num2_rand].bit[i] = bit_temp;
			}

		}
	}
	return true;
}

// �������������
bool CGeneticAlgorithmBase::Mutation()
{
	int position_rand = 0;
	int i = 0;
	double rate_rand = 0.0;
	//�������������
	srand((unsigned)time(NULL));
	//��������趨Ϊ��Ⱥ����
	for (i = 0; i<m_npopulation; i++)
	{
		rate_rand = (float)rand() / (RAND_MAX);
		//������ڽ�����ʾͽ��б������
		if (rate_rand <= m_dmutate_rate)
		{
			//�������ͻ��λ��
			position_rand = (int)rand() % (m_nDNA_size);
			//double dmutationvalue = m_poprange[int(double(rand()) / RAND_MAX*(m_nrangelen - 1))];
			int xh = rand()%(m_nrangelen);
			//ͻ��
			//m_spop[i].bit[position_rand] = !m_spop[i].bit[position_rand];
			m_spop[i].bit[position_rand] = m_poprange[xh];
		}

	}
	return true;
}

bool CGeneticAlgorithmBase::seletc_prw()
{
	Chromosome *spopcur = new (std::nothrow) Chromosome[m_npopulation];
	if ( !spopcur )
		return false;
	for (int k = 0; k < m_npopulation; k++)
	{
		m_spop[k].clone(spopcur[k]);
	}
	
	int i = 0, j = 0;
	double rate_rand = 0.0;
	m_best = spopcur[0];
	//�������������
	srand((unsigned)time(NULL));
	for (i = 0; i < m_npopulation; i++)
	{
		rate_rand = (float)rand() / (RAND_MAX);
		if (rate_rand < spopcur[0].cumu_fit)
		{
			m_spop[i] = spopcur[0];
			m_spop[i].fitness = 0.0;
			m_spop[i].cumu_fit = 0.0;
			m_spop[i].rate_fit = 0.0;
		}
		else
		{
			for (j = 0; j < m_npopulation-1; j++)
			{
				if (spopcur[j].cumu_fit <= rate_rand && rate_rand < spopcur[j + 1].cumu_fit)
				{
					m_spop[i] = spopcur[j+1];
					m_spop[i].fitness = 0.0;
					m_spop[i].cumu_fit = 0.0;
					m_spop[i].rate_fit = 0.0;
					break;
				}
			}
		}

		//�����ǰ�����Ŀǰ�����и��廹Ҫ���㣬�򽫵�ǰ������Ϊ���Ÿ���
		if (spopcur[i].fitness > m_best.fitness)
			m_best = spopcur[i];
	}
	delete []spopcur;
	return true;

}

bool CGeneticAlgorithmBase::fresh_property()
{
	int j = 0;
	double sum = 0.0;
	bool bRes = Objective();
	if ( !bRes )
		return bRes;
	for (j = 0; j < m_npopulation; j++)
	{

		//Ⱦɫ����룬�������ƻ���Ϊʮ���ƣ��õ�һ������ֵ
		//��������ƴ���Ӧ��10������ֵ
		//decode(population_current[j]);
		//����Ⱦɫ�����Ӧ��
		//population_current[j].fitness = Objective(m_spop[j].bit);
		sum = sum + m_spop[j].fitness;

	}


	//����ÿ��Ⱦɫ�����Ӧֵ�ٷֱȼ��ۼ���Ӧ��ֵ�İٷֱȣ������̶�ѡ��ʱ����ѡ��Ⱦɫ��  
	m_spop[0].rate_fit = m_spop[0].fitness / sum;
	m_spop[0].cumu_fit = m_spop[0].rate_fit;
	for (j = 1; j < m_npopulation; j++)
	{
		m_spop[j].rate_fit = m_spop[j].fitness / sum;
		m_spop[j].cumu_fit = m_spop[j].rate_fit + m_spop[j - 1].cumu_fit;
	}
	return bRes;

}

bool CGeneticAlgorithmBase::Generate()
{
	bool bRes = InitPopulation();
	if ( !bRes )
		return bRes;
	bRes = fresh_property();
	if ( !bRes )
		return bRes;
	clock_t tstart = clock();
	for (int i = 0; i < m_niterations; i++)
	{
		clock_t tend = clock();
		if ( double(tend-tstart)/1000.0 >= m_nTerminateSec )
			break;
		//��ѡ�����������µ���Ⱥ
		bRes = seletc_prw();
		if ( !bRes )
			break;
		//��ѡ������Ⱥ���н������
		bRes = Cross();
		if ( !bRes )
			break;
		//�Խ�������Ⱥ���б������
		bRes = Mutation();
		if ( !bRes )
			break;
		//������Ⱥ�ڸ��������ֵ
		bRes = fresh_property();
		if ( !bRes )
			break;
	}
	return bRes;

}

Chromosome CGeneticAlgorithmBase::getBest() const
{
	return m_best;
}

