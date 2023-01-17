import pickle
import streamlit as st
from tmdbv3api import Movie, TMDb

movie = Movie()     # 객체 만들어서 초기화
tmdb = TMDb()       # 객체 만들어서 초기화
tmdb.api_key='2fe66b147bbb5646a0f5a58f85ac0172' # api 발급
tmdb.language = 'ko=KR' # 한국어로 설정

def get_recommendations(title):
    # 영화 제목을 통해서 전체 데이터 기준 그 영화의 index 값을 얻기
    idx = movies[movies['title'] == title].index[0]
    
    # 코사인 유사도 매트릭스 [cosine_sim]에서 idx 에 해당하는 데이터를 [idx, 유사도] 형태로 얻기
    sim_scores = list(enumerate(cosine_sim[idx]))
    
    # 코사인 유사도 기준으로 내림차순 정렬
    sim_scores = sorted(sim_scores, key=lambda x:x[1], reverse=True)
    
    # 자기 자신을 제외한 10개의 추천 영화를 슬라이싱
    sim_scores = sim_scores[1:11]
    
    # 추천 영화목록 10개의 인텍스 정보 추출
    movie_indices = [i[0] for i in sim_scores]
    
    # 인덱스 정보를 통해 영화 제목 추출
    images = []
    titles = []
    for i in movie_indices:
        id = movies['id'].iloc[i]
        details = movie.details(id) # movie 정보들 가져오기
        
        image_path = details['poster_path']
        if image_path:                           # 이미지가 없는 것 예외처리
            image_path = 'https://image.tmdb.org/t/p/w500' + image_path # 이미지 가져오기
        else :
            image_path = 'no_image.jpg'

        images.append(image_path)
        titles.append(details['title'])

    return images, titles

# 저장했던 파일 열고 읽어오기
movies = pickle.load(open('movies.pickle', 'rb'))
cosine_sim = pickle.load(open('cosine_sim.pickle', 'rb'))

st.set_page_config(layout='wide') # 전체화면 설정
st.header('Hyungjunflix') # 헤드 설정

movie_list = movies['title'].values
title = st.selectbox('Choose a movie you like', movie_list)     # 콤보박스 만들기
if st.button('Recommend'):              # 버튼 만들기
    with st.spinner('Please wait...'):   # progress bar 생성
        images, titles = get_recommendations(title)
        
        idx = 0                     # 웹 페이지에 images, titles 표시
        for i in range(0, 2):
            cols = st.columns(5)    # 5개의 column 생성
            for col in cols:
                col.image(images[idx])
                col.write(titles[idx])
                idx += 1