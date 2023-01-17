#!/usr/bin/env python
# coding: utf-8

# ## 3. Collaborative Filtering (협업 필터링: 사용자 리뷰 기반)

# In[4]:


# surprise 패키지 이용
import surprise
surprise.__version__


# In[5]:


import pandas as pd
from surprise import Reader, Dataset, SVD
from surprise.model_selection import cross_validate


# In[6]:


ratings = pd.read_csv('ratings_small.csv')
ratings.head()


# In[7]:


ratings['rating'].min()
# 최소값


# In[8]:


ratings['rating'].max()
# 최대값


# In[9]:


reader = Reader(rating_scale=(0.5, 5)) # Reader 객체 생성 (최소값 : 0.5 , 최대값 : 5.0)


# In[14]:


data = Dataset.load_from_df(ratings[['userId', 'movieId', 'rating']], reader = reader) # 3개의 column 에 관하여 데이터 읽어오기
data


# In[16]:


svd = SVD(random_state=0) # SVD 모델 사용


# In[17]:


# cross_validate 함수를 통해 데이터 평가 (교차검증)
cross_validate(svd, data, measures=['RMSE', 'MAE'], cv=5, verbose=True) 


# In[ ]:


# 전체 데이터를 사용하여 학습


# In[ ]:


trainset = data.build_full_trainset()
svd.fit(trainset)


# In[18]:


# usetId = 1 에 대하여 예측해보기
ratings[ratings['userId']==1]


# In[21]:


svd.predict(1, 1029, 3) # userId 1번인 사람이 MovieId = 1029인 영화에 대해서 실제 평가 3점일 때, 예측 평가 점수?


# In[23]:


ratings[ratings['userId'] == 100]


# In[26]:


svd.predict(100, 1029) # userId = 100, MovieId = 1029 예측해보기

