import { useState, useEffect } from "react";
import { Link } from "react-router-dom";
import "../styles/pager.css";

const Pager = ({ items, pageCount }) => {
    const [pageItems, setPageitems] = useState(items);
    const [count, setCount] = useState(pageCount);
    const [filteredItems, setFiltered] = useState([]);
    const [current, setCurrent] = useState(1);
    const [total, setTotal] = useState(Math.ceil(pageItems.length / count));
    const [bar, setBar] = useState([]);

    useEffect(() => {
        calculate(1);
    }, []);

    useEffect(() => {
        //console.log("page items changed", items);
        setPageitems(items);
    },[items]);

    useEffect(() => {
        console.log("change pelase");
        calculate(1);
    }, [pageItems]);

    const calculate = (pageNo) => {
        //console.log("page", pageNo);
        let currentPage = pageNo;
        let totalPages = Math.ceil(pageItems.length / count);
        if (currentPage > totalPages) currentPage = totalPages;
        let hasPreviousPage = currentPage === 1 ? false : true;
        let hasNextPage = currentPage === totalPages ? false : true;
        let first = (currentPage - 1) * count;
        let last = first + count;
        let filteredItems = pageItems.slice(first, last);

        setFiltered(filteredItems);
        setCurrent(currentPage);
        setTotal(totalPages);
    }

    const pageClick = (e, pageNo) => {
        //calculate(pageNo);
        if (current === 1 && pageNo <= 1) {
            setCurrent(1);
        } else if (current === total && pageNo > total) {
            setCurrent(total);
        } else {
            setCurrent(pageNo);
        }
    }

    useEffect(() => {

        //console.log("current", current);
        var pageArray = [];
        for (var i = 1; i <= total; i++) {
            pageArray.push(i);
        }

        const pages = pageArray.map((idx) => {
            return (
                <li> <Link to="#" key={idx} onClick={(e) => pageClick(e, idx)} className={`item ${current === idx ? "active" : null}`}> {idx} </Link> </li>
            )
        });
        setBar(pages);
        calculate(current);

    }, [current]);

    return (
        <div>
            <div className="paginate">
                <ul>
                    {
                        filteredItems.map((item) => {
                            return (
                                <li>
                                    <Link key={item.recipe_id} to={`/recipe/${item.recipe_id}`}> {item.recipe_name} </Link>
                                </li>
                            );
                        })
                    }
                </ul>
            </div>

            <div className=""> 
            <ul className="">
                <li className="item"> <Link to="#" onClick={(e) => pageClick(e, current-1)}> prev </Link> </li>
                {bar}
                <li className="item"> <Link to="#" onClick={(e) => pageClick(e, current+1)}> next </Link> </li>
            </ul>
            </div>
        </div>
    );
}

export default Pager;