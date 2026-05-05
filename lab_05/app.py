from __future__ import annotations

from pathlib import Path

import pandas as pd
import plotly.express as px
import streamlit as st


DATA_FILE = Path("vhi_cleaned_all.csv")
INDICATORS = ("VCI", "TCI", "VHI")
REGION_FIELD = "region_id"
YEAR_FIELD = "year"
WEEK_FIELD = "week"


st.set_page_config(
    page_title="Лабораторна робота №5",
    page_icon="📊",
    layout="wide",
)


@st.cache_data(show_spinner="Завантаження даних...")
def read_dataset(csv_path: Path) -> pd.DataFrame:
    """Read and lightly validate the source CSV file."""
    if not csv_path.exists():
        st.error(
            "Не знайдено файл vhi_cleaned_all.csv. "
            "Покладіть його в одну папку з app.py та перезапустіть застосунок."
        )
        st.stop()

    data = pd.read_csv(csv_path)
    required_columns = {REGION_FIELD, YEAR_FIELD, WEEK_FIELD, *INDICATORS}
    missing_columns = required_columns.difference(data.columns)

    if missing_columns:
        st.error("У CSV відсутні потрібні колонки: " + ", ".join(sorted(missing_columns)))
        st.stop()

    return data.dropna(subset=[REGION_FIELD, YEAR_FIELD, WEEK_FIELD])


def filter_dataset(
    data: pd.DataFrame,
    selected_region: int | str,
    years: tuple[int, int],
    weeks: tuple[int, int],
) -> pd.DataFrame:
    """Return rows matching sidebar selections."""
    mask = (
        (data[REGION_FIELD] == selected_region)
        & (data[YEAR_FIELD].between(years[0], years[1]))
        & (data[WEEK_FIELD].between(weeks[0], weeks[1]))
    )
    return data.loc[mask].copy()


def sort_data(data: pd.DataFrame, column: str, mode: str) -> pd.DataFrame:
    """Sort selected data according to the user option."""
    if mode == "За зростанням":
        return data.sort_values(column, ascending=True)
    if mode == "За спаданням":
        return data.sort_values(column, ascending=False)
    return data


st.title("Лабораторна робота №5")
st.markdown("### ЗПАД")
st.caption("Інтерактивний веб-додаток для аналізу показників VCI, TCI та VHI")

source_df = read_dataset(DATA_FILE)

available_regions = sorted(source_df[REGION_FIELD].unique().tolist())
year_limits = (int(source_df[YEAR_FIELD].min()), int(source_df[YEAR_FIELD].max()))
week_limits = (int(source_df[WEEK_FIELD].min()), int(source_df[WEEK_FIELD].max()))

with st.sidebar:
    st.header("Панель керування")

    chosen_metric = st.selectbox("Часовий ряд", INDICATORS, index=2)
    chosen_region = st.selectbox("Область", available_regions)

    chosen_years = st.slider(
        "Діапазон років",
        min_value=year_limits[0],
        max_value=year_limits[1],
        value=year_limits,
    )

    chosen_weeks = st.slider(
        "Діапазон тижнів",
        min_value=week_limits[0],
        max_value=week_limits[1],
        value=week_limits,
    )

    sort_mode = st.radio(
        "Сортування таблиці",
        ("Без сортування", "За зростанням", "За спаданням"),
        horizontal=False,
    )

    if st.button("Скинути фільтри", use_container_width=True):
        st.rerun()

selected_region_df = filter_dataset(source_df, chosen_region, chosen_years, chosen_weeks)
selected_region_df = sort_data(selected_region_df, chosen_metric, sort_mode)
period_df = source_df[
    source_df[YEAR_FIELD].between(chosen_years[0], chosen_years[1])
    & source_df[WEEK_FIELD].between(chosen_weeks[0], chosen_weeks[1])
].copy()

summary_left, summary_middle, summary_right = st.columns(3)
summary_left.metric("Кількість записів", len(selected_region_df))
summary_middle.metric("Обрана область", chosen_region)
summary_right.metric("Показник", chosen_metric)

st.divider()

tab_table, tab_line, tab_compare, tab_help = st.tabs(
    ["Відфільтровані дані", "Графік часового ряду", "Порівняння областей", "Інструкція"]
)

with tab_table:
    st.subheader("Таблиця після застосування фільтрів")
    st.dataframe(selected_region_df, use_container_width=True, hide_index=True)

with tab_line:
    st.subheader(f"Динаміка {chosen_metric} для області {chosen_region}")
    if selected_region_df.empty:
        st.warning("Для вибраних параметрів немає даних.")
    else:
        line_chart = px.line(
            selected_region_df.sort_values([YEAR_FIELD, WEEK_FIELD]),
            x=WEEK_FIELD,
            y=chosen_metric,
            color=YEAR_FIELD,
            markers=True,
            title=f"{chosen_metric}: зміна показника за тижнями",
            labels={WEEK_FIELD: "Тиждень", chosen_metric: chosen_metric, YEAR_FIELD: "Рік"},
        )
        st.plotly_chart(line_chart, use_container_width=True)

with tab_compare:
    st.subheader(f"Середнє значення {chosen_metric} за областями")
    if period_df.empty:
        st.warning("Для вибраного періоду немає даних.")
    else:
        comparison_df = (
            period_df.groupby(REGION_FIELD, as_index=False)[chosen_metric]
            .mean()
            .sort_values(chosen_metric, ascending=False)
        )
        bar_chart = px.bar(
            comparison_df,
            x=REGION_FIELD,
            y=chosen_metric,
            title=f"Порівняння областей за середнім {chosen_metric}",
            labels={REGION_FIELD: "Область", chosen_metric: f"Середній {chosen_metric}"},
        )
        st.plotly_chart(bar_chart, use_container_width=True)

with tab_help:
    st.subheader("Як користуватися застосунком")
    st.markdown(
        """
        1. У лівій панелі виберіть показник: **VCI**, **TCI** або **VHI**.
        2. Оберіть область, для якої потрібно переглянути часовий ряд.
        3. Задайте інтервали років і тижнів за допомогою слайдерів.
        4. За потреби увімкніть сортування таблиці.
        5. Перемикайте вкладки, щоб переглянути таблицю, графік для області та порівняння областей.
        6. Кнопка **Скинути фільтри** повертає застосунок до початкового стану.
        """
    )
