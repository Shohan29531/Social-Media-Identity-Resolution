# Identity Resolution Across Social Media Platforms

This repository contains the code and data used for our approach to identity resolution across social media platforms. The work focuses on leveraging topics, sentiments, and timings of posts to match user profiles across platforms such as Disqus and Twitter.

## Overview

The primary goal of this project is to develop methods for identifying and linking profiles across social media platforms using publicly available post data. We collected posts from approximately 5,000 user profiles on Disqus and Twitter and conducted analyses to match their identities across the two platforms.

### Key Features

- **Temporal and Non-Temporal Analysis:**
  - Explored both temporal and non-temporal methods for matching profiles.
  - Temporal methods generally outperformed non-temporal ones.
  - Found that the size of the temporal window influenced results more significantly than the shifting amount.

- **Sentiment Analysis:**
  - Examined the impact of sentiment inclusion in the matching process.
  - Found that sentiment data made little difference due to issues with data extraction quality.

- **Distance-Based Scoring Model:**
  - Implemented a reward-and-punishment-focused scoring model.
  - Achieved an accuracy of **24.198%** and an average rank of **158.217** out of 2525 profiles in the dataset.

## Results

While neither temporal nor non-temporal methods proved definitively superior, our findings suggest:
- Temporal approaches benefit significantly from carefully tuned window sizes.
- Sentiment inclusion requires further refinement, such as evaluating sentiments by topic, to have a meaningful impact.

## Future Work

1. **Refining Sentiment Analysis:**
   - Incorporating sentiment evaluation per topic to improve data quality and insights.
2. **Extending Temporal Analysis:**
   - Introducing additional phases to strengthen temporal matching techniques.
3. **Improving Scoring Model:**
   - Adjusting weights and refining reward mechanisms for better accuracy and ranking performance.

## Citation

<pre>
If you use this work, please cite as:
@article{islam2024temporal,
  title={A Temporal Psycholinguistics Approach to Identity Resolution of Social Media Users},
  author={Islam, Md Touhidul},
  journal={arXiv preprint arXiv:2407.19967},
  year={2024}
}
<\pre>
